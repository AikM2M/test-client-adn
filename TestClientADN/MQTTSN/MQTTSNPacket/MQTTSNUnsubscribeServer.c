/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "StackTrace.h"
#include "MQTTSNPacket.h"

int MQTTSNDeserialize_unsubscribe(unsigned short* packetid, MQTTSN_topicid* topicFilter,
		unsigned char* buf, int buflen)
{
	MQTTSNFlags flags;
	unsigned char* curdata = buf;
	unsigned char* enddata = NULL;
	int rc = 0;
	int mylen = 0;

	FUNC_ENTRY;
	curdata += (rc = MQTTSNPacket_decode(curdata, buflen, &mylen)); /* read length */
	enddata = buf + mylen;
	if (enddata - curdata > buflen)
		goto exit;

	if (MQTTSNPacket_readChar(&curdata) != MQTTSN_UNSUBSCRIBE)
		goto exit;

	flags.all = MQTTSNPacket_readChar(&curdata);
	*packetid = MQTTSNPacket_readInt(&curdata);

	topicFilter->type = flags.bits.topicIdType;
	if (topicFilter->type == MQTTSN_TOPIC_TYPE_NORMAL)
	{
		topicFilter->data.long_.len = enddata - curdata;
		topicFilter->data.long_.name = (char*)curdata;
	}
	else if (topicFilter->type == MQTTSN_TOPIC_TYPE_PREDEFINED)
		topicFilter->data.id = MQTTSNPacket_readInt(&curdata);
	else if (topicFilter->type == MQTTSN_TOPIC_TYPE_SHORT)
	{
		topicFilter->data.short_name[0] = MQTTSNPacket_readChar(&curdata);
		topicFilter->data.short_name[1] = MQTTSNPacket_readChar(&curdata);
	}

	rc = 1;
exit:
	FUNC_EXIT_RC(rc);
	return rc;
}


int MQTTSNSerialize_unsuback(unsigned char* buf, int buflen, unsigned short packetid)
{
	unsigned char *ptr = buf;
	int len = 0;
	int rc = 0;

	FUNC_ENTRY;
	if ((len = MQTTSNPacket_len(7)) > buflen)
	{
		rc = MQTTSNPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}
	ptr += MQTTSNPacket_encode(ptr, len); /* write length */
	MQTTSNPacket_writeChar(&ptr, MQTTSN_UNSUBACK);      /* write message type */

	MQTTSNPacket_writeInt(&ptr, packetid);

	rc = ptr - buf;
exit:
	FUNC_EXIT_RC(rc);
	return rc;
}



