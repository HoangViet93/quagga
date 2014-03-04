/*
 * IS-IS Rout(e)ing protocol - netlink.c
 *
 * Copyright 2014 Gandi, SAS.  All rights reserved.
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public Licenseas published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "isisd/netlink.h"

static struct nla_policy TRILL_U16_POLICY [TRILL_ATTR_MAX + 1]={
  [TRILL_ATTR_U16] = {.type = NLA_U16},
};
static struct nla_policy TRILL_U32_POLICY [TRILL_ATTR_MAX + 1]={
  [TRILL_ATTR_U32] = {.type = NLA_U32},
};
static struct nla_policy TRILL_STRING_POLICY [TRILL_ATTR_MAX + 1]={
  [TRILL_ATTR_STRING] = {.type = NLA_STRING},
};
static struct nla_policy TRILL_BIN_POLICY [TRILL_ATTR_MAX + 1]={
  [TRILL_ATTR_BIN] = {.type = NLA_UNSPEC},
};

static struct nla_policy TRILL_VNI_POLICY [TRILL_ATTR_MAX + 1]={
  [TRILL_ATTR_U16] = {.type = NLA_U16},
  [TRILL_ATTR_BIN] = {.type = NLA_UNSPEC},
};
int parse_cb(struct nl_msg *msg, void *data)
{
  struct genlmsghdr* genlh;
  struct trill_nl_header *tnlh;
  struct nlmsghdr *nlh = nlmsg_hdr(msg);
  struct nlattr *attrs[TRILL_ATTR_MAX + 1];
  struct isis_area *area = (struct isis_area *) data;
  /* Validate message and parse attributes */
  genlh = nlmsg_data(nlh);
  tnlh = (struct trill_nl_header *)genlmsg_data(genlh);
  if(tnlh->ifindex != KERNL_RESPONSE_INTERFACE)
    return 0;
  switch (genlh->cmd){
    case TRILL_CMD_SET_DESIG_VLAN:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		     TRILL_ATTR_MAX, TRILL_U32_POLICY);
      break;
    }
    case TRILL_CMD_SET_NICKS_INFO:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_BIN_POLICY);
      break;
    }
    case TRILL_CMD_GET_NICKS_INFO:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_BIN_POLICY);
      break;
    }
    case TRILL_CMD_ADD_NICKS_INFO:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_BIN_POLICY);
      break;
    }
    case TRILL_CMD_DEL_NICK:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, NULL);
      break;
    }
    case TRILL_CMD_SET_TREEROOT_ID:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_U16_POLICY);
      break;
    }
    case TRILL_CMD_NEW_BRIDGE:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_STRING_POLICY);
      break;
    }
    case TRILL_CMD_GET_BRIDGE:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_U16_POLICY);
      break;
    }
    case TRILL_CMD_SET_BRIDGE:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_U16_POLICY);
      break;
    }
    case TRILL_CMD_LIST_NICK:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_BIN_POLICY);
      break;
    }
    case TRILL_CMD_PORT_FLUSH:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, NULL);
      break;
    }
    case TRILL_CMD_NICK_FLUSH:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_U16_POLICY);
      break;
    }
    case TRILL_CMD_GET_VNIS:
    {
      genlmsg_parse(nlh, sizeof(struct trill_nl_header), attrs,
		    TRILL_ATTR_MAX, TRILL_VNI_POLICY);
      break;
    }
    default:
    {
      zlog_warn("received unknown command\n");
      break;
    }
  }
  return 0;
}