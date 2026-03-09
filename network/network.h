#ifndef NETWORK_H
#define NETWORK_H
#include "./socket/socket.h"
struct sockaddr {
	short   sa_family;
	char	sa_data[14];
};

/* Supported address families. */
#define AF_UNSPEC	0
#define AF_UNIX		1	/* Unix domain sockets 		*/
#define AF_LOCAL	1	/* POSIX name for AF_UNIX	*/
#define AF_INET		2	/* Internet IP Protocol 	*/
#define AF_AX25		3	/* Amateur Radio AX.25 		*/
#define AF_IPX		4	/* Novell IPX 			*/
#define AF_APPLETALK	5	/* AppleTalk DDP 		*/
#define AF_NETROM	6	/* Amateur Radio NET/ROM 	*/
#define AF_BRIDGE	7	/* Multiprotocol bridge 	*/
#define AF_ATMPVC	8	/* ATM PVCs			*/
#define AF_X25		9	/* Reserved for X.25 project 	*/
#define AF_INET6	10	/* IP version 6			*/
#define AF_ROSE		11	/* Amateur Radio X.25 PLP	*/
#define AF_DECnet	12	/* Reserved for DECnet project	*/
#define AF_NETBEUI	13	/* Reserved for 802.2LLC project*/
#define AF_SECURITY	14	/* Security callback pseudo AF */
#define AF_KEY		15      /* PF_KEY key management API */
#define AF_NETLINK	16
#define AF_ROUTE	AF_NETLINK /* Alias to emulate 4.4BSD */
#define AF_PACKET	17	/* Packet family		*/
#define AF_ASH		18	/* Ash				*/
#define AF_ECONET	19	/* Acorn Econet			*/
#define AF_ATMSVC	20	/* ATM SVCs			*/
#define AF_RDS		21	/* RDS sockets 			*/
#define AF_SNA		22	/* Linux SNA Project (nutters!) */
#define AF_IRDA		23	/* IRDA sockets			*/
#define AF_PPPOX	24	/* PPPoX sockets		*/
#define AF_WANPIPE	25	/* Wanpipe API Sockets */
#define AF_LLC		26	/* Linux LLC			*/
#define AF_IB		27	/* Native InfiniBand address	*/
#define AF_MPLS		28	/* MPLS */
#define AF_CAN		29	/* Controller Area Network      */
#define AF_TIPC		30	/* TIPC sockets			*/
#define AF_BLUETOOTH	31	/* Bluetooth sockets 		*/
#define AF_IUCV		32	/* IUCV sockets			*/
#define AF_RXRPC	33	/* RxRPC sockets 		*/
#define AF_ISDN		34	/* mISDN sockets 		*/
#define AF_PHONET	35	/* Phonet sockets		*/
#define AF_IEEE802154	36	/* IEEE802154 sockets		*/
#define AF_CAIF		37	/* CAIF sockets			*/
#define AF_ALG		38	/* Algorithm sockets		*/
#define AF_NFC		39	/* NFC sockets			*/
#define AF_VSOCK	40	/* vSockets			*/
#define AF_KCM		41	/* Kernel Connection Multiplexor*/
#define AF_QIPCRTR	42	/* Qualcomm IPC Router          */
#define AF_SMC		43	/* smc sockets: reserve number for
				 * PF_SMC protocol family that
				 * reuses AF_INET address family
				 */
#define AF_XDP		44	/* XDP sockets			*/
#define AF_MCTP		45	/* Management component
				 * transport protocol
				 */

#define AF_MAX		46	/* For now.. */

/* Protocol families, same as address families. */
#define PF_UNSPEC	AF_UNSPEC
#define PF_UNIX		AF_UNIX
#define PF_LOCAL	AF_LOCAL
#define PF_INET		AF_INET
#define PF_AX25		AF_AX25
#define PF_IPX		AF_IPX
#define PF_APPLETALK	AF_APPLETALK
#define	PF_NETROM	AF_NETROM
#define PF_BRIDGE	AF_BRIDGE
#define PF_ATMPVC	AF_ATMPVC
#define PF_X25		AF_X25
#define PF_INET6	AF_INET6
#define PF_ROSE		AF_ROSE
#define PF_DECnet	AF_DECnet
#define PF_NETBEUI	AF_NETBEUI
#define PF_SECURITY	AF_SECURITY
#define PF_KEY		AF_KEY
#define PF_NETLINK	AF_NETLINK
#define PF_ROUTE	AF_ROUTE
#define PF_PACKET	AF_PACKET
#define PF_ASH		AF_ASH
#define PF_ECONET	AF_ECONET
#define PF_ATMSVC	AF_ATMSVC
#define PF_RDS		AF_RDS
#define PF_SNA		AF_SNA
#define PF_IRDA		AF_IRDA
#define PF_PPPOX	AF_PPPOX
#define PF_WANPIPE	AF_WANPIPE
#define PF_LLC		AF_LLC
#define PF_IB		AF_IB
#define PF_MPLS		AF_MPLS
#define PF_CAN		AF_CAN
#define PF_TIPC		AF_TIPC
#define PF_BLUETOOTH	AF_BLUETOOTH
#define PF_IUCV		AF_IUCV
#define PF_RXRPC	AF_RXRPC
#define PF_ISDN		AF_ISDN
#define PF_PHONET	AF_PHONET
#define PF_IEEE802154	AF_IEEE802154
#define PF_CAIF		AF_CAIF
#define PF_ALG		AF_ALG
#define PF_NFC		AF_NFC
#define PF_VSOCK	AF_VSOCK
#define PF_KCM		AF_KCM
#define PF_QIPCRTR	AF_QIPCRTR
#define PF_SMC		AF_SMC
#define PF_XDP		AF_XDP
#define PF_MCTP		AF_MCTP
#define PF_MAX		AF_MAX

#define SOCK_STREAM	1
#define SOCK_DGRAM	2
#define SOCK_RAW	3
#define SOCK_RDM	4
#define SOCK_SEQPACKET	5
#define SOCK_DCCP	6
#define SOCK_PACKET	10


#define SOL_SOCKET	1

#define SO_DEBUG	1
#define SO_REUSEADDR	2
#define SO_TYPE		3
#define SO_ERROR	4
#define SO_DONTROUTE	5
#define SO_BROADCAST	6
#define SO_SNDBUF	7
#define SO_RCVBUF	8
#define SO_SNDBUFFORCE	32
#define SO_RCVBUFFORCE	33
#define SO_KEEPALIVE	9
#define SO_OOBINLINE	10
#define SO_NO_CHECK	11
#define SO_PRIORITY	12
#define SO_LINGER	13
#define SO_BSDCOMPAT	14
#define SO_REUSEPORT	15
#ifndef SO_PASSCRED /* powerpc only differs in these */
#define SO_PASSCRED	16
#define SO_PEERCRED	17
#define SO_RCVLOWAT	18
#define SO_SNDLOWAT	19
#define SO_RCVTIMEO_OLD	20
#define SO_SNDTIMEO_OLD	21
#endif

/* Security levels - as per NRL IPv6 - don't actually do anything */
#define SO_SECURITY_AUTHENTICATION		22
#define SO_SECURITY_ENCRYPTION_TRANSPORT	23
#define SO_SECURITY_ENCRYPTION_NETWORK		24

#define SO_BINDTODEVICE	25

/* Socket filtering */
#define SO_ATTACH_FILTER	26
#define SO_DETACH_FILTER	27
#define SO_GET_FILTER		SO_ATTACH_FILTER

#define SO_PEERNAME		28

#define SO_ACCEPTCONN		30

#define SO_PEERSEC		31
#define SO_PASSSEC		34

#define SO_MARK			36

#define SO_PROTOCOL		38
#define SO_DOMAIN		39

#define SO_RXQ_OVFL             40

#define SO_WIFI_STATUS		41
#define SCM_WIFI_STATUS	SO_WIFI_STATUS
#define SO_PEEK_OFF		42

/* Instruct lower device to use last 4-bytes of skb data as FCS */
#define SO_NOFCS		43

#define SO_LOCK_FILTER		44

#define SO_SELECT_ERR_QUEUE	45

#define SO_BUSY_POLL		46

#define SO_MAX_PACING_RATE	47

#define SO_BPF_EXTENSIONS	48

#define SO_INCOMING_CPU		49

#define SO_ATTACH_BPF		50
#define SO_DETACH_BPF		SO_DETACH_FILTER

#define SO_ATTACH_REUSEPORT_CBPF	51
#define SO_ATTACH_REUSEPORT_EBPF	52

#define SO_CNX_ADVICE		53

#define SCM_TIMESTAMPING_OPT_STATS	54

#define SO_MEMINFO		55

#define SO_INCOMING_NAPI_ID	56

#define SO_COOKIE		57

#define SCM_TIMESTAMPING_PKTINFO	58

#define SO_PEERGROUPS		59

#define SO_ZEROCOPY		60

#define SO_TXTIME		61
#define SCM_TXTIME		SO_TXTIME

#define SO_BINDTOIFINDEX	62

#define SO_TIMESTAMP_OLD        29
#define SO_TIMESTAMPNS_OLD      35
#define SO_TIMESTAMPING_OLD     37

#define SO_TIMESTAMP_NEW        63
#define SO_TIMESTAMPNS_NEW      64
#define SO_TIMESTAMPING_NEW     65

#define SO_RCVTIMEO_NEW         66
#define SO_SNDTIMEO_NEW         67

#define SO_DETACH_REUSEPORT_BPF 68

#define SO_PREFER_BUSY_POLL	69
#define SO_BUSY_POLL_BUDGET	70

#define SO_NETNS_COOKIE		71

#define SO_BUF_LOCK		72

#define SO_RESERVE_MEM		73

#define SO_TXREHASH		74

#define SO_RCVMARK		75

#define SO_PASSPIDFD		76
#define SO_PEERPIDFD		77

#define SO_DEVMEM_LINEAR	78
#define SCM_DEVMEM_LINEAR	SO_DEVMEM_LINEAR
#define SO_DEVMEM_DMABUF	79
#define SCM_DEVMEM_DMABUF	SO_DEVMEM_DMABUF
#define SO_DEVMEM_DONTNEED	80

#define SCM_TS_OPT_ID		81

#define SO_RCVPRIORITY		82

#define SO_PASSRIGHTS		83

#define SO_INQ			84
#define SCM_INQ			SO_INQ

enum {
  IPPROTO_IP = 0,		/* Dummy protocol for TCP		*/
#define IPPROTO_IP		IPPROTO_IP
  IPPROTO_ICMP = 1,		/* Internet Control Message Protocol	*/
#define IPPROTO_ICMP		IPPROTO_ICMP
  IPPROTO_IGMP = 2,		/* Internet Group Management Protocol	*/
#define IPPROTO_IGMP		IPPROTO_IGMP
  IPPROTO_IPIP = 4,		/* IPIP tunnels (older KA9Q tunnels use 94) */
#define IPPROTO_IPIP		IPPROTO_IPIP
  IPPROTO_TCP = 6,		/* Transmission Control Protocol	*/
#define IPPROTO_TCP		IPPROTO_TCP
  IPPROTO_EGP = 8,		/* Exterior Gateway Protocol		*/
#define IPPROTO_EGP		IPPROTO_EGP
  IPPROTO_PUP = 12,		/* PUP protocol				*/
#define IPPROTO_PUP		IPPROTO_PUP
  IPPROTO_UDP = 17,		/* User Datagram Protocol		*/
#define IPPROTO_UDP		IPPROTO_UDP
  IPPROTO_IDP = 22,		/* XNS IDP protocol			*/
#define IPPROTO_IDP		IPPROTO_IDP
  IPPROTO_TP = 29,		/* SO Transport Protocol Class 4	*/
#define IPPROTO_TP		IPPROTO_TP
  IPPROTO_DCCP = 33,		/* Datagram Congestion Control Protocol */
#define IPPROTO_DCCP		IPPROTO_DCCP
  IPPROTO_IPV6 = 41,		/* IPv6-in-IPv4 tunnelling		*/
#define IPPROTO_IPV6		IPPROTO_IPV6
  IPPROTO_RSVP = 46,		/* RSVP Protocol			*/
#define IPPROTO_RSVP		IPPROTO_RSVP
  IPPROTO_GRE = 47,		/* Cisco GRE tunnels (rfc 1701,1702)	*/
#define IPPROTO_GRE		IPPROTO_GRE
};
#define TCP_NODELAY		1	/* Turn off Nagle's algorithm. */
#define TCP_MAXSEG		2	/* Limit MSS */
#define TCP_CORK		3	/* Never send partially complete segments */
#define TCP_KEEPIDLE		4	/* Start keeplives after this period */
#define TCP_KEEPINTVL		5	/* Interval between keepalives */
#define TCP_KEEPCNT		6	/* Number of keepalives before death */
#define TCP_SYNCNT		7	/* Number of SYN retransmits */
#define TCP_LINGER2		8	/* Life time of orphaned FIN-WAIT-2 state */
#define TCP_DEFER_ACCEPT	9	/* Wake up listener only when data arrive */
#define TCP_WINDOW_CLAMP	10	/* Bound advertised window */
#define TCP_INFO		11	/* Information about this connection. */
#define TCP_QUICKACK		12	/* Block/reenable quick acks */
#define TCP_CONGESTION		13	/* Congestion control algorithm */
#define TCP_MD5SIG		14	/* TCP MD5 Signature (RFC2385) */
#define TCP_THIN_LINEAR_TIMEOUTS 16	/* Use linear timeouts for thin streams*/
#define TCP_THIN_DUPACK		17	/* Fast retrans. after 1 dupack */
#define TCP_USER_TIMEOUT	18	/* How long for loss retry before timeout */
#define TCP_REPAIR		19	/* TCP sock is under repair right now */
#define TCP_REPAIR_QUEUE	20
#define TCP_QUEUE_SEQ		21
#define TCP_REPAIR_OPTIONS	22
#define TCP_FASTOPEN		23	/* Enable FastOpen on listeners */
#define TCP_TIMESTAMP		24
#define TCP_NOTSENT_LOWAT	25	/* limit number of unsent bytes in write queue */
#define TCP_CC_INFO		26	/* Get Congestion Control (optional) info */
#define TCP_SAVE_SYN		27	/* Record SYN headers for new connections */
#define TCP_SAVED_SYN		28	/* Get SYN headers recorded for connection */
#define TCP_REPAIR_WINDOW	29	/* Get/set window parameters */
#define TCP_FASTOPEN_CONNECT	30	/* Attempt FastOpen with connect */
#define TCP_ULP			31	/* Attach a ULP to a TCP connection */
#define TCP_MD5SIG_EXT		32	/* TCP MD5 Signature with extensions */
#define TCP_FASTOPEN_KEY	33	/* Set the key for Fast Open (cookie) */
#define TCP_FASTOPEN_NO_COOKIE	34	/* Enable TFO without a TFO cookie */
#define TCP_ZEROCOPY_RECEIVE	35
#define TCP_INQ			36	/* Notify bytes available to read as a cmsg on read */

#define TCP_CM_INQ		TCP_INQ

#define TCP_TX_DELAY		37	/* delay outgoing packets by XX usec */

#define TCP_AO_ADD_KEY		38	/* Add/Set MKT */
#define TCP_AO_DEL_KEY		39	/* Delete MKT */
#define TCP_AO_INFO		40	/* Set/list TCP-AO per-socket options */
#define TCP_AO_GET_KEYS		41	/* List MKT(s) */
#define TCP_AO_REPAIR		42	/* Get/Set SNEs and ISNs */

#define TCP_IS_MPTCP		43	/* Is MPTCP being used? */
#define TCP_RTO_MAX_MS		44	/* max rto time in ms */
#define TCP_RTO_MIN_US		45	/* min rto time in us */
#define TCP_DELACK_MAX_US	46	/* max delayed ack time in us */

#define TCP_REPAIR_ON		1
#define TCP_REPAIR_OFF		0
#define TCP_REPAIR_OFF_NO_WP	-1	/* Turn off without window probes */

#endif
