#ifndef NET_H_
#define NET_H_

#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* IP protocols defined in RFC 1700 */
enum protocol_type
{
    PROTO_reserved = 0,
    PROTO_ICMP = 1,
    PROTO_IGMP = 2,
    PROTO_GGP = 3,
    PROTO_IP = 4,
    PROTO_ST = 5,
    PROTO_TCP = 6,
    PROTO_UCL = 7,
    PROTO_EGP = 8,
    PROTO_IGP = 9,
    PROTO_BBN_RCC_MON = 10,
    PROTO_NVP_II = 11,
    PROTO_PUP = 12,
    PROTO_ARGUS = 13,
    PROTO_EMCON = 14,
    PROTO_XNET = 15,
    PROTO_CHAOS = 16,
    PROTO_UDP = 17,
    PROTO_MUX = 18,
    PROTO_DCN_MEAS = 19,
    PROTO_HMP = 20,
    PROTO_PRM = 21,
    PROTO_XNS_IDP = 22,
    PROTO_TRUNK_1 = 23,
    PROTO_TRUNK_2 = 24,
    PROTO_LEAF_1 = 25,
    PROTO_LEAF_2 = 26,
    PROTO_RDP = 27,
    PROTO_IRTP = 28,
    PROTO_ISO_TP4 = 29,
    PROTO_NETBLT = 30,
    PROTO_MFE_NSP = 31,
    PROTO_MERIT_INP = 32,
    PROTO_SEP = 33,
    PROTO_3PC = 34,
    PROTO_IDPR = 35,
    PROTO_XTP = 36,
    PROTO_DDP = 37,
    PROTO_IDPR_CMTP = 38,
    PROTO_TPPP = 39,
    PROTO_IL = 40,
    PROTO_SIP = 41,
    PROTO_SDRP = 42,
    PROTO_SIP_SR = 43,
    PROTO_SIP_FRAG = 44,
    PROTO_IDRP = 45,
    PROTO_RSVP = 46,
    PROTO_GRE = 47,
    PROTO_MHRP = 48,
    PROTO_BNA = 49,
    PROTO_SIPP_ESP = 50,
    PROTO_SIPP_AH = 51,
    PROTO_I_NLSP = 52,
    PROTO_SWIPE = 53,
    PROTO_NHRP = 54,
    PROTO_anyhost = 61,
    PROTO_CFTP = 62,
    PROTO_anylocal = 63,
    PROTO_SAT_EXPAK = 64,
    PROTO_KRYPTOLAN = 65,
    PROTO_RVD = 66,
    PROTO_IPPC = 67,
    PROTO_anydistfs = 68,
    PROTO_SAT_MON = 69,
    PROTO_VISA = 70,
    PROTO_IPCV = 71,
    PROTO_CPNX = 72,
    PROTO_CPHB = 73,
    PROTO_WSN = 74,
    PROTO_BR_SAT_MON = 76,
    PROTO_SUN_ND = 77,
    PROTO_WB_MON = 78,
    PROTO_WB_EXPAK = 79,
    PROTO_ISO_IP = 80,
    PROTO_VMTP = 81,
    PROTO_SECURE_VMTP = 82,
    PROTO_VINES = 83,
    PROTO_TTP = 84,
    PROTO_NSFNET_IGP = 85,
    PROTO_DGP = 86,
    PROTO_TCF = 87,
    PROTO_IGRP = 88,
    PROTO_OSPFIGP = 89,
    PROTO_SPRITE_RPC = 90,
    PROTO_LARP = 91,
    PROTO_MTP = 92,
    PROTO_AX_25 = 93,
    PROTO_IPIP = 94,
    PROTO_MICP = 95,
    PROTO_SSC_SP = 96,
    PROTO_ETHERIP = 97,
    PROTO_ENCAP = 98,
    PROTO_anypriv = 99,
    PROTO_GMTP = 100,
    PROTO_reserved2 = 255
};

struct socketinfo
{
    int fd;
    int domain;
    int type;
    int protocol;
    bool reuse;
};

struct serverinfo
{
    in_addr_t addr;
    int port;
    enum protocol_type protocol;
    struct socketinfo socket_info;
    FILE* listen_fp;
};

struct clientinfo
{
    in_addr_t addr;
    int port;
    char* hostname;
};

int startserver_mp(struct serverinfo* server_info,
        int (callback)(void*, size_t, void**, struct clientinfo*));

#endif /* NET_H_ */

