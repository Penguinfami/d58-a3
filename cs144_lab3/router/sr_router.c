/**********************************************************************
 * file:  sr_router.c
 * date:  Mon Feb 18 12:50:42 PST 2002
 * Contact: casado@stanford.edu
 *
 * Description:
 *
 * This file contains all the functions that interact directly
 * with the routing table, as well as the main entry method
 * for routing.
 *
 **********************************************************************/

#include <stdio.h>
#include <assert.h>


#include "sr_if.h"
#include "sr_rt.h"
#include "sr_router.h"
#include "sr_protocol.h"
#include "sr_arpcache.h"
#include "sr_utils.h"

/*---------------------------------------------------------------------
 * Method: sr_init(void)
 * Scope:  Global
 *
 * Initialize the routing subsystem
 *
 *---------------------------------------------------------------------*/

void sr_init(struct sr_instance* sr)
{
    /* REQUIRES */
    assert(sr);

    /* Initialize cache and cache cleanup thread */
    sr_arpcache_init(&(sr->cache));

    pthread_attr_init(&(sr->attr));
    pthread_attr_setdetachstate(&(sr->attr), PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&(sr->attr), PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setscope(&(sr->attr), PTHREAD_SCOPE_SYSTEM);
    pthread_t thread;

    pthread_create(&thread, &(sr->attr), sr_arpcache_timeout, sr);
    
    /* Add initialization code here! */

} /* -- sr_init -- */

/*---------------------------------------------------------------------
 * Method: sr_handlepacket(uint8_t* p,char* interface)
 * Scope:  Global
 *
 * This method is called each time the router receives a packet on the
 * interface.  The packet buffer, the packet length and the receiving
 * interface are passed in as parameters. The packet is complete with
 * ethernet headers.
 *
 * Note: Both the packet buffer and the character's memory are handled
 * by sr_vns_comm.c that means do NOT delete either.  Make a copy of the
 * packet instead if you intend to keep it around beyond the scope of
 * the method call.
 *
 *---------------------------------------------------------------------*/
void longest_prefix_match(struct sr_rt * table, uint32_t ip, uint32_t *longest_match, uint32_t *shortest_diff, char *interface) {
  
  *shortest_diff = 0;
  struct sr_rt *cur = table;
  int foundMatch = 0;
  while (cur != NULL) {
    uint32_t diff = htonl(cur->dest.s_addr) & htonl(cur->mask.s_addr);
    if (((diff == htonl(ip) & htonl(cur->mask.s_addr)) && diff > *shortest_diff)) {
      *shortest_diff = diff;
      *longest_match = cur->gw.s_addr;
      memcpy(interface, cur->interface, sr_IFACE_NAMELEN);
      foundMatch = 1;
    }
    cur = cur->next;
  }
}
int nextId = 1;

uint8_t * send_ICMP_message(int send, struct sr_instance *sr, unsigned int len, const char*iface, sr_icmp_t3_hdr_t * icmp_payload, uint8_t *payload, uint16_t id, uint8_t tos, uint32_t src, uint32_t dst, uint8_t srcMAC[ETHER_ADDR_LEN], 
  uint8_t hostMAC[ETHER_ADDR_LEN], uint8_t type, uint8_t code);

void sendICMPErrorMessage(struct sr_instance *sr, uint8_t *orig, unsigned int len, unsigned char *interface, int type, int code) {
  sr_ethernet_hdr_t * ether_header = (sr_ethernet_hdr_t *)orig;

  unsigned int ip_start = sizeof( sr_ethernet_hdr_t);
  unsigned int ip_len = sizeof( sr_ethernet_hdr_t) + sizeof( sr_ip_hdr_t);
  unsigned int icmp_len = sizeof( sr_ethernet_hdr_t) + sizeof( sr_ip_hdr_t) + sizeof(sr_icmp_t3_hdr_t);

  uint32_t dst_ip;
  unsigned char dst_addr[ETHER_ADDR_LEN];
  sr_icmp_t3_hdr_t icmp_header;
  if (ether_header->ether_type == htons(ethertype_ip)) {
      sr_ip_hdr_t * orig_ip = (sr_ip_hdr_t *)(orig + ip_start);
      int payloadExtra = (len - ip_len > 8 ? 8 : len - ip_len);
      memcpy(icmp_header.data, orig_ip, sizeof(sr_ip_hdr_t) + payloadExtra);
      dst_ip = orig_ip->ip_src;
  } else {
      sr_arp_hdr_t * orig_arp = (sr_arp_hdr_t *)(orig + ip_start);
      memcpy(icmp_header.data, orig_arp, sizeof(sr_arp_hdr_t));
      dst_ip = orig_arp->ar_sip;
  }
  struct sr_if * intface = sr_get_interface(sr, interface);
  struct sr_arpentry * arplookup = sr_arpcache_lookup(&sr->cache, dst_ip);
  sendOrARPRequest(sr, dst_ip,
      send_ICMP_message(1, sr, icmp_len, interface, &icmp_header, NULL, NULL, 0, intface->ip, 
          dst_ip, intface->addr, intface->addr, type, code),
      intface->name, icmp_len);     
}

void sr_handlepacket(struct sr_instance* sr,
        uint8_t * packet/* lent */,
        unsigned int len,
        char* interface/* lent */)
{
  /* REQUIRES */
  assert(sr);
  assert(packet);
  assert(interface);

  printf("*** -> Received packet of length %d \n",len);

  unsigned int arp_len = sizeof( sr_ethernet_hdr_t) + sizeof( sr_arp_hdr_t);
  unsigned int ip_len = sizeof( sr_ethernet_hdr_t) + sizeof( sr_ip_hdr_t);
  unsigned int icmp_len = sizeof( sr_ethernet_hdr_t) + sizeof( sr_ip_hdr_t) + sizeof(sr_icmp_t3_hdr_t);
  /* fill in code here */
  struct sr_rt *rt = sr->routing_table;
  struct sr_if *intface = sr_get_interface(sr, interface);
  if (intface != NULL) {
      sr_ethernet_hdr_t * ether_header = ( sr_ethernet_hdr_t *)packet;
      enum sr_ethertype etype = ethertype(packet);
      if (etype == ethertype_ip) {
        uint8_t * ip_packet = packet + sizeof( sr_ethernet_hdr_t);
        struct sr_ip_hdr *ip_header = ( sr_ip_hdr_t *)ip_packet;
        uint32_t dest_ip = ip_header->ip_dst;
        /* sanity check */
        int failsSanityCheck = 0;
        uint8_t * ip_packet_data = ip_packet + sizeof( sr_ip_hdr_t);
        size_t header_size =  sizeof( sr_ethernet_hdr_t) + sizeof( sr_ip_hdr_t);
        if (len < header_size) {
          failsSanityCheck = 1;
        }
        uint16_t receivedCK = ip_header->ip_sum;
        ip_header->ip_sum = 0;
        uint16_t ck = cksum(ip_header, sizeof( sr_ip_hdr_t));
        if (ck != receivedCK) {
          failsSanityCheck = 1;
        }
        if (failsSanityCheck == 0) {
          ip_header->ip_ttl -= 1;
          ip_header->ip_sum = 0;
          ip_header->ip_sum = cksum(ip_packet, sizeof( sr_ip_hdr_t));

          int reachedDest = 0;
          /* check our interfaces: */
          struct sr_if *currentIF = sr->if_list;
          unsigned char foundMAC[ETHER_ADDR_LEN];
          int found = 0;
          uint32_t ip_diff;
          uint32_t longest_matching_ip = dest_ip;
          char longest_matching_iface[sr_IFACE_NAMELEN];
          memcpy(longest_matching_iface, interface, sr_IFACE_NAMELEN);

          while(currentIF != NULL && found == 0) {
            if (currentIF->ip == dest_ip) {
              reachedDest = 1;
              found = 1;
              memcpy(foundMAC, currentIF->addr, ETHER_ADDR_LEN); 
              memcpy(longest_matching_iface, currentIF->name, sr_IFACE_NAMELEN); 
              longest_matching_ip = currentIF->ip;
            }
            currentIF = currentIF->next;
          }
          
          if (found == 0) {
            longest_prefix_match(rt, dest_ip, &longest_matching_ip, &ip_diff, longest_matching_iface);
            
            if (ip_diff != 0) {
              struct sr_arpentry * arplookup = sr_arpcache_lookup(&sr->cache, longest_matching_ip);
              if (arplookup != NULL) {
                found = 1;
                memcpy(foundMAC, arplookup->mac, ETHER_ADDR_LEN);
                free(arplookup);
              }
            } 
            else {
              /* icmp network unreachable */
              sendICMPErrorMessage(sr, packet, len, interface, 3, 0);    
            } 
          }
          
          
          struct sr_if *longestMatchingIntface = sr_get_interface(sr, longest_matching_iface);

          if (reachedDest == 1) {
          /* reached its destination */
            if (ip_protocol(ip_packet) == ip_protocol_icmp) {
              uint8_t * icmp_data = packet + sizeof(sr_ethernet_hdr_t)  + sizeof(sr_ip_hdr_t);
              sr_icmp_t3_hdr_t * icmp_header = (sr_icmp_t3_hdr_t *)icmp_data;
              if (icmp_header->icmp_type == 8 && icmp_header->icmp_code == 0) {
                if (ip_header->ip_ttl <= 0) {
                  sendICMPErrorMessage(sr, packet, len, longestMatchingIntface->name, 11, 0);
                  return;
                }
                /* echo request */
                longest_prefix_match(rt, ip_header->ip_src, &longest_matching_ip, &ip_diff, longest_matching_iface);
                longestMatchingIntface = sr_get_interface(sr, longest_matching_iface);
                struct sr_arpentry * arplookup = sr_arpcache_lookup(&sr->cache, ip_header->ip_src);
                if (arplookup != NULL) {
                  found = 1;
                  memcpy(foundMAC, arplookup->mac, ETHER_ADDR_LEN);
                  free(arplookup);
                }
                uint8_t *newPacket = send_ICMP_message(1, sr, len, interface, icmp_header, packet + icmp_len, icmp_header->unused, ip_header->ip_tos, dest_ip, 
                    ip_header->ip_src, intface->addr, intface->addr, 0, 0);
                sendOrARPRequest(sr, ip_header->ip_src,newPacket,interface, len);                  
              } else if (icmp_header->icmp_type == 3 && icmp_header->icmp_code == 1) {
                /* ARP expired */
                if (len >= 2 * icmp_len) {
                  /* payload is an icmp request */
                  uint8_t * payload = packet + icmp_len;
                  uint8_t * payload_ip_packet = payload + sizeof(sr_ethernet_hdr_t);
                  sr_ip_hdr_t * payload_ip_packet_header = (sr_ip_hdr_t *)payload_ip_packet;
                  uint32_t srcIp = payload_ip_packet_header->ip_src;
                  struct sr_arpentry * arplookup = sr_arpcache_lookup(&sr->cache, ip_header->ip_src);
                  if (arplookup != NULL) {
                    found = 1;
                    memcpy(foundMAC, arplookup->mac, ETHER_ADDR_LEN);
                    free(arplookup);
                  }
                }
              }
            } else if (ip_protocol(ip_packet) == 6 || ip_protocol(ip_packet) == 17) {
              /* TCP or UDP, so sent icmp port unreachable */
              sendICMPErrorMessage(sr, packet, len, interface, 3, 3);    
            }
          } else {
            if (ip_header->ip_ttl <= 0) {
              sendICMPErrorMessage(sr, packet, len, interface, 11, 0);
              return;
            }
            /* not reached its destination, send the packet forward */
            memcpy(ether_header->ether_dhost, foundMAC, ETHER_ADDR_LEN);
            memcpy(ether_header->ether_shost, longestMatchingIntface->addr, ETHER_ADDR_LEN);
            sendOrARPRequest(sr, longest_matching_ip, packet, longestMatchingIntface->name, len);
          }
        } else {
          /* failed sanity check */
          uint8_t * newPacket = send_ICMP_message(1,sr, icmp_len, interface, NULL, NULL, ip_header->ip_id, ip_header->ip_tos, intface->ip, 
                  ip_header->ip_src, ether_header->ether_dhost, ether_header->ether_shost, 3, 1);
          sendOrARPRequest(sr, ip_header->ip_src, newPacket, interface, icmp_len);
        }
      } else if ((etype == ethertype_arp)) {
        /* arp request */
        uint8_t * arp_packet = packet + sizeof(sr_ethernet_hdr_t);
        struct sr_arp_hdr *arp_header = ( sr_arp_hdr_t *)arp_packet;
        if (arp_header->ar_op == htons(arp_op_request)) {
          uint32_t target_ip = arp_header->ar_tip;
          unsigned char foundMAC[ETHER_ADDR_LEN];
          int found = 0;
          if (target_ip == intface->ip) {
            memcpy(foundMAC, intface->addr, ETHER_ADDR_LEN);
            found = 1;
          } else {
            unsigned char mac_addr[ETHER_ADDR_LEN];
            memcpy(mac_addr, arp_header->ar_tha, ETHER_ADDR_LEN);
            struct sr_arpreq *req = sr_arpcache_insert(&sr->cache, arp_header->ar_sha, arp_header->ar_sip);
            if (req != NULL) {
              free(req);
            }
            /* lookup target */
            struct sr_arpentry *arp = sr_arpcache_lookup(&sr->cache, arp_header->ar_tip);
            if (arp != NULL) {
              memcpy(foundMAC, arp->mac, ETHER_ADDR_LEN);
              found = 1;
            }
          }
          if (found == 1) {
            unsigned int totalLen = sizeof(sr_ethernet_hdr_t) + sizeof(sr_arp_hdr_t);
            uint8_t * new_packet = (uint8_t *)malloc(totalLen);
            sr_ethernet_hdr_t * new_ether_header = (sr_ethernet_hdr_t *)new_packet;
            memcpy(new_ether_header->ether_dhost, arp_header->ar_sha, ETHER_ADDR_LEN);
            memcpy(new_ether_header->ether_shost, intface->addr, ETHER_ADDR_LEN);
            new_ether_header->ether_type = htons(ethertype_arp);
            uint8_t * new_arp_packet = new_packet + sizeof(sr_ethernet_hdr_t);
            struct sr_arp_hdr *new_arp_header = (sr_arp_hdr_t *)new_arp_packet;
            *new_arp_header = *arp_header; /* i think header should be the same ? */
            new_arp_header->ar_op = htons(arp_op_reply);

            memcpy(new_arp_header->ar_sha, intface->addr, ETHER_ADDR_LEN);
            new_arp_header->ar_sip = intface->ip;
            memcpy(new_arp_header->ar_tha, foundMAC, ETHER_ADDR_LEN);
            new_arp_header->ar_tip = arp_header->ar_sip;

            struct sr_arpreq *req = sr_arpcache_insert(&sr->cache, foundMAC, arp_header->ar_tip);
            if (req != NULL) {
              sr_arpreq_destroy(&sr->cache, req);
            }
            memcpy(new_packet, new_ether_header, sizeof(sr_ethernet_hdr_t));
            memcpy(new_packet + sizeof(sr_ethernet_hdr_t), new_arp_header, sizeof(sr_arp_hdr_t));

            sr_send_packet(sr, new_packet, totalLen, interface);
          }
        } else if (arp_header->ar_op == htons(arp_op_reply)){
          /* arp reply */
          uint32_t receivedIp = arp_header->ar_sip;
          unsigned char receivedMAC[ETHER_ADDR_LEN];
          memcpy(receivedMAC, arp_header->ar_sha, ETHER_ADDR_LEN);
          struct sr_arpreq *req = sr_arpcache_insert(&sr->cache, receivedMAC, receivedIp);
          if (req != NULL) {
            struct sr_packet *cur = req->packets;
            int idx = 0;
            while(cur != NULL) {
                struct sr_packet *next = cur->next;
                idx++;
                sr_ethernet_hdr_t * new_ether_header = (sr_ethernet_hdr_t *)(cur->buf);
                memcpy(new_ether_header->ether_dhost, receivedMAC, ETHER_ADDR_LEN);
                struct sr_if *reqInterface = sr_get_interface(sr, cur->iface);
                memcpy(new_ether_header->ether_shost, reqInterface->addr, ETHER_ADDR_LEN);
                sr_send_packet(sr, cur->buf, cur->len, cur->iface);
                cur = next;
            }
            sr_arpreq_destroy(&sr->cache, req);

          }
        }
      }
  }
}/* end sr_ForwardPacket */


