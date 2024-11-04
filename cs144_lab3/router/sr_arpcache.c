#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include "sr_arpcache.h"
#include "sr_router.h"
#include "sr_if.h"
#include "sr_protocol.h"


void sendOrARPRequest(struct sr_instance *sr, uint32_t dest_ip, uint8_t * packet, unsigned char *interface, unsigned int len) {
    sr_ethernet_hdr_t * ether_header = (sr_ethernet_hdr_t *)packet;
    struct sr_arpentry * arplookup = sr_arpcache_lookup(&sr->cache, dest_ip);
    if (arplookup != NULL) {
        memcpy(ether_header->ether_dhost, arplookup->mac, ETHER_ADDR_LEN);
        sr_send_packet(sr, packet, len, interface);
        free(arplookup);
    } else {
        struct sr_arpreq *req = sr_arpcache_queuereq(&sr->cache, dest_ip, packet, len, interface);
        if (req->times_sent == 0) {
            sendARPRequest(sr, req, sr_get_interface(sr, interface));
        }
        req->times_sent++;
    }   
}
void sendARPRequest(struct sr_instance *sr, struct sr_arpreq *req, struct sr_if * intface) {
    int totalLen = sizeof(sr_ethernet_hdr_t) + sizeof(struct sr_arp_hdr);
    uint8_t *packet = (uint8_t *)malloc(totalLen);
    sr_ethernet_hdr_t * ether_header = (sr_ethernet_hdr_t *)packet;
    uint8_t * arp_packet = packet + sizeof(sr_ethernet_hdr_t);
    sr_arp_hdr_t * arp_header = (sr_arp_hdr_t *)arp_packet;
    int i;
    for (i = 0; i < ETHER_ADDR_LEN; i++) {
        ether_header->ether_dhost[i] = 0xff;
    }
    ether_header->ether_type = htons(ethertype_arp);
    memcpy(ether_header->ether_shost, intface->addr, ETHER_ADDR_LEN);
    arp_header->ar_hrd = htons(arp_hrd_ethernet);
    arp_header->ar_pro = htons(2048); /* idk */
    arp_header->ar_hln = ETHER_ADDR_LEN;
    arp_header->ar_pln = 4;
    arp_header->ar_op = htons(arp_op_request);
    memcpy(arp_header->ar_sha, intface->addr, ETHER_ADDR_LEN);
    arp_header->ar_sip = intface->ip;
    memcpy(arp_header->ar_tha, ether_header->ether_dhost, ETHER_ADDR_LEN);
    arp_header->ar_tip = req->ip;

    sr_send_packet(sr, packet, totalLen, intface->name);
}

uint8_t * send_ICMP_message(int send, struct sr_instance *sr, unsigned int len, const char*iface, sr_icmp_t3_hdr_t * icmp_payload, uint8_t *payload, uint16_t id, uint8_t tos, uint32_t src, uint32_t dst, uint8_t srcMAC[ETHER_ADDR_LEN], 
  uint8_t hostMAC[ETHER_ADDR_LEN], uint8_t type, uint8_t code) {
  unsigned int totalLen = sizeof(struct sr_ethernet_hdr) 
    + sizeof(sr_ip_hdr_t) + sizeof(sr_icmp_t3_hdr_t);
  int lenDiff = len - totalLen;
  totalLen += lenDiff;
  uint8_t * packet = (uint8_t *)malloc(totalLen);
  sr_ethernet_hdr_t * ether_header = (sr_ethernet_hdr_t *)packet;
  uint8_t * ip_packet = packet + sizeof(sr_ethernet_hdr_t);
  struct sr_ip_hdr *ip_header = (sr_ip_hdr_t *)ip_packet;
  uint8_t * icmp_data = packet + sizeof(sr_ethernet_hdr_t)  + sizeof(sr_ip_hdr_t);
  sr_icmp_t3_hdr_t * icmp_header = (sr_icmp_t3_hdr_t *)icmp_data;
  
  if (payload != NULL) {
    memcpy(icmp_data + sizeof(sr_icmp_t3_hdr_t), payload, lenDiff);
  }

  /* icmp header*/
  icmp_header->icmp_type = type;
  icmp_header->icmp_code = code;
  icmp_header->unused = id;
  if (icmp_payload != NULL) {
    icmp_header->next_mtu = icmp_payload->next_mtu;
    memcpy(icmp_header->data, icmp_payload->data, ICMP_DATA_SIZE);
  }
  icmp_header->icmp_sum = 0;
  icmp_header->icmp_sum = cksum(icmp_header, sizeof(sr_icmp_t3_hdr_t) + lenDiff);

  /* ip header */
  ip_header->ip_ttl = INIT_TTL;
  ip_header->ip_p = ip_protocol_icmp;
  ip_header->ip_src = src;
  ip_header->ip_dst = dst;
  ip_header->ip_tos = tos;
  uint16_t myLen = totalLen - sizeof(sr_ethernet_hdr_t);
  ip_header->ip_len = htons(myLen);
  ip_header->ip_id = id;
  ip_header->ip_off = 0;
  ip_header->ip_sum = 0;
  ip_header->ip_hl = sizeof(sr_arp_hdr_t) / 4 - 2;
  ip_header->ip_v = 4;

  /* ethernet header */
  memcpy(ether_header->ether_dhost, hostMAC, ETHER_ADDR_LEN);
  memcpy(ether_header->ether_shost, srcMAC, ETHER_ADDR_LEN);
  ether_header->ether_type = htons(ethertype_ip);

  ip_header->ip_sum = cksum(ip_packet, myLen);

  return packet;
}

void sendICMPErrorMessage(struct sr_instance *sr, uint8_t *orig, unsigned int len, unsigned char *interface, int type, int code);

/* 
  This function gets called every second. For each request sent out, we keep
  checking whether we should resend an request or destroy the arp request.
  See the comments in the header file for an idea of what it should look like.
*/
void sr_arpcache_sweepreqs(struct sr_instance *sr) { 
    /* Fill this in */
     struct sr_arpcache *cache = &sr->cache;
    if (cache->requests != NULL) {
        struct sr_arpreq *req = cache->requests;
        while(req != NULL) {
            time_t curTime = time(0);
            struct sr_arpentry *entry = sr_arpcache_lookup(cache, req->ip);
            if (entry != NULL) {
                free(entry);
                sr_arpreq_destroy(cache, req);
                req = cache->requests;
            } else if ((req->sent == 0 || req->sent > curTime - 15) && req->times_sent < 5) {
                req->times_sent++;
                req->sent = curTime;
                struct sr_packet *cur = req->packets;
                while(cur != NULL) {
                    struct sr_packet *next = cur->next;
                    sendARPRequest(sr, req, sr_get_interface(sr, cur->iface));
                    cur = next;
                    break;
                }
                req = req->next;
            } else {
                struct sr_packet *curPacket = req->packets;
                while(curPacket != NULL) {
                    sendICMPErrorMessage(sr, curPacket->buf, curPacket->len, curPacket->iface, 3, 1);    
                    curPacket = curPacket->next;
                }
                
                sr_arpreq_destroy(cache, req);
                req = cache->requests;
            }
        }
    }
}

/* You should not need to touch the rest of this code. */

/* Checks if an IP->MAC mapping is in the cache. IP is in network byte order.
   You must free the returned structure if it is not NULL. */
struct sr_arpentry *sr_arpcache_lookup(struct sr_arpcache *cache, uint32_t ip) {
    pthread_mutex_lock(&(cache->lock));
    
    struct sr_arpentry *entry = NULL, *copy = NULL;
    
    int i;
    for (i = 0; i < SR_ARPCACHE_SZ; i++) {
        if ((cache->entries[i].valid) && (cache->entries[i].ip == ip)) {
            entry = &(cache->entries[i]);
        }
    }
    
    /* Must return a copy b/c another thread could jump in and modify
       table after we return. */
    if (entry) {
        copy = (struct sr_arpentry *) malloc(sizeof(struct sr_arpentry));
        memcpy(copy, entry, sizeof(struct sr_arpentry));
    }
        
    pthread_mutex_unlock(&(cache->lock));
    
    return copy;
}

/* Adds an ARP request to the ARP request queue. If the request is already on
   the queue, adds the packet to the linked list of packets for this sr_arpreq
   that corresponds to this ARP request. You should free the passed *packet.
   
   A pointer to the ARP request is returned; it should not be freed. The caller
   can remove the ARP request from the queue by calling sr_arpreq_destroy. */
struct sr_arpreq *sr_arpcache_queuereq(struct sr_arpcache *cache,
                                       uint32_t ip,
                                       uint8_t *packet,           /* borrowed */
                                       unsigned int packet_len,
                                       char *iface)
{
    pthread_mutex_lock(&(cache->lock));
    
    struct sr_arpreq *req;
    for (req = cache->requests; req != NULL; req = req->next) {
        if (req->ip == ip) {
            break;
        }
    }
    
    /* If the IP wasn't found, add it */
    if (!req) {
        req = (struct sr_arpreq *) calloc(1, sizeof(struct sr_arpreq));
        req->ip = ip;
        req->next = cache->requests;
        cache->requests = req;
    }
    
    /* Add the packet to the list of packets for this request */
    if (packet && packet_len && iface) {
        struct sr_packet *new_pkt = (struct sr_packet *)malloc(sizeof(struct sr_packet));
        
        new_pkt->buf = (uint8_t *)malloc(packet_len);
        memcpy(new_pkt->buf, packet, packet_len);
        new_pkt->len = packet_len;
		new_pkt->iface = (char *)malloc(sr_IFACE_NAMELEN);
        strncpy(new_pkt->iface, iface, sr_IFACE_NAMELEN);
        new_pkt->next = req->packets;
        req->packets = new_pkt;
    }

    pthread_mutex_unlock(&(cache->lock));
    
    return req;
}

/* This method performs two functions:
   1) Looks up this IP in the request queue. If it is found, returns a pointer
      to the sr_arpreq with this IP. Otherwise, returns NULL.
   2) Inserts this IP to MAC mapping in the cache, and marks it valid. */
struct sr_arpreq *sr_arpcache_insert(struct sr_arpcache *cache,
                                     unsigned char *mac,
                                     uint32_t ip)
{
    pthread_mutex_lock(&(cache->lock));
    
    struct sr_arpreq *req, *prev = NULL, *next = NULL; 
    for (req = cache->requests; req != NULL; req = req->next) {
        if (req->ip == ip) {            
            if (prev) {
                next = req->next;
                prev->next = next;
            } 
            else {
                next = req->next;
                cache->requests = next;
            }
            
            break;
        }
        prev = req;
    }
    
    int i;
    for (i = 0; i < SR_ARPCACHE_SZ; i++) {
        if (!(cache->entries[i].valid))
            break;
    }
    
    if (i != SR_ARPCACHE_SZ) {
        memcpy(cache->entries[i].mac, mac, 6);
        cache->entries[i].ip = ip;
        cache->entries[i].added = time(NULL);
        cache->entries[i].valid = 1;
    }
    
    pthread_mutex_unlock(&(cache->lock));
    
    return req;
}

/* Frees all memory associated with this arp request entry. If this arp request
   entry is on the arp request queue, it is removed from the queue. */
void sr_arpreq_destroy(struct sr_arpcache *cache, struct sr_arpreq *entry) {
    pthread_mutex_lock(&(cache->lock));
    
    if (entry) {
        struct sr_arpreq *req, *prev = NULL, *next = NULL; 
        for (req = cache->requests; req != NULL; req = req->next) {
            if (req == entry) {                
                if (prev) {
                    next = req->next;
                    prev->next = next;
                } 
                else {
                    next = req->next;
                    cache->requests = next;
                }
                
                break;
            }
            prev = req;
        }
        
        struct sr_packet *pkt, *nxt;
        
        for (pkt = entry->packets; pkt; pkt = nxt) {
            nxt = pkt->next;
            if (pkt->buf)
                free(pkt->buf);
            if (pkt->iface)
                free(pkt->iface);
            free(pkt);
        }
        
        free(entry);
    }
    
    pthread_mutex_unlock(&(cache->lock));
}

/* Prints out the ARP table. */
void sr_arpcache_dump(struct sr_arpcache *cache) {
    fprintf(stderr, "\nMAC            IP         ADDED                      VALID\n");
    fprintf(stderr, "-----------------------------------------------------------\n");
    
    int i;
    for (i = 0; i < SR_ARPCACHE_SZ; i++) {
        struct sr_arpentry *cur = &(cache->entries[i]);
        unsigned char *mac = cur->mac;
        fprintf(stderr, "%.1x%.1x%.1x%.1x%.1x%.1x   %.8x   %.24s   %d\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ntohl(cur->ip), ctime(&(cur->added)), cur->valid);
    }
    
    fprintf(stderr, "\n");
}

/* Initialize table + table lock. Returns 0 on success. */
int sr_arpcache_init(struct sr_arpcache *cache) {  
    /* Seed RNG to kick out a random entry if all entries full. */
    srand(time(NULL));
    
    /* Invalidate all entries */
    memset(cache->entries, 0, sizeof(cache->entries));
    cache->requests = NULL;
    
    /* Acquire mutex lock */
    pthread_mutexattr_init(&(cache->attr));
    pthread_mutexattr_settype(&(cache->attr), PTHREAD_MUTEX_RECURSIVE);
    int success = pthread_mutex_init(&(cache->lock), &(cache->attr));
    
    return success;
}

/* Destroys table + table lock. Returns 0 on success. */
int sr_arpcache_destroy(struct sr_arpcache *cache) {
    return pthread_mutex_destroy(&(cache->lock)) && pthread_mutexattr_destroy(&(cache->attr));
}

/* Thread which sweeps through the cache and invalidates entries that were added
   more than SR_ARPCACHE_TO seconds ago. */
void *sr_arpcache_timeout(void *sr_ptr) {
    struct sr_instance *sr = sr_ptr;
    struct sr_arpcache *cache = &(sr->cache);
    
    while (1) {
        sleep(1.0);
        
        pthread_mutex_lock(&(cache->lock));
    
        time_t curtime = time(NULL);
        
        int i;    
        for (i = 0; i < SR_ARPCACHE_SZ; i++) {
            if ((cache->entries[i].valid) && (difftime(curtime,cache->entries[i].added) > SR_ARPCACHE_TO)) {
                cache->entries[i].valid = 0;
            }
        }
        
        sr_arpcache_sweepreqs(sr);

        pthread_mutex_unlock(&(cache->lock));
    }
    
    return NULL;
}

