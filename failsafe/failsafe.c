/* SPDX-License-Identifier:	GPL-2.0 */
/*
 * Copyright (C) 2019 MediaTek Inc. All Rights Reserved.
 *
 * Author: Weijie Gao <weijie.gao@mediatek.com>
 *
 */

#include <common.h>
#include <malloc.h>
#include <net/tcp.h>
#include <net/httpd.h>
#if defined(CONFIG_MTK_DHCPD)
#include <net/mtk_dhcpd.h>
#endif
#include <u-boot/md5.h>
#include <stdlib.h>

#include "fs.h"

static u32 upload_data_id;
static const void *upload_data;
static size_t upload_size;
static int upgrade_success;
static int upload_type;

extern int write_firmware_failsafe(size_t data_addr, uint32_t data_size);
extern int write_bootloader_failsafe(size_t data_addr, uint32_t data_size);
extern int erase_nvram_failsafe(void);
extern int erase_factory_failsafe(void);
extern int erase_factory2_failsafe(void);
extern int erase_nvram_failsafe_ex(void);

static int output_plain_file(struct httpd_response *response,
	const char *filename)
{
	const struct fs_desc *file;
	int ret = 0;

	file = fs_find_file(filename);

	response->status = HTTP_RESP_STD;

	if (file) {
		response->data = file->data;
		response->size = file->size;
	} else {
		response->data = "Error: file not found";
		response->size = strlen(response->data);
		ret = 1;
	}

	response->info.code = 200;
	response->info.connection_close = 1;
	response->info.content_type = "text/html";

	return ret;
}

static void index_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW)
		output_plain_file(response, "index.html");
}

static void upload_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	char *buff, *md5_ptr, *size_ptr, size_str[16];
	u8 md5_sum[16];
	struct httpd_form_value *fw;
	const struct fs_desc *file;
	int i;

	static char hexchars[] = "0123456789abcdef";

	if (status == HTTP_CB_NEW) {
		fw = httpd_request_find_value(request, "firmware");
		upload_type = 0;

		if (!fw) {
			struct httpd_form_value *type_val = httpd_request_find_value(request, "upload_type");
			if (type_val) {
				unsigned long tval = simple_strtoul(type_val->data, NULL, 10);
				if (tval == 3) {
					upload_type = 3;
					if (output_plain_file(response, "flashing_nvram.html")) {
						response->info.code = 500;
						return;
					}
					upload_data_id = upload_id;
					upload_data = NULL;
					upload_size = 0;
					return;
				}
			}
			response->info.code = 302;
			response->info.connection_close = 1;
			response->info.location = "/";
			return;
		}

		if (output_plain_file(response, "upload.html")) {
			response->info.code = 500;
			return;
		}

		buff = malloc(response->size + 1);
		if (buff) {
			memcpy(buff, response->data, response->size);
			buff[response->size] = 0;

			md5_ptr = strstr(buff, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
			size_ptr = strstr(buff, "YYYYYYYYYY");

			if (md5_ptr) {
				md5((u8 *) fw->data, fw->size, md5_sum);
				for (i = 0; i < 16; i++) {
					u8 hex;

					hex = (md5_sum[i] >> 4) & 0xf;
					md5_ptr[i * 2] = hexchars[hex];
					hex = md5_sum[i] & 0xf;
					md5_ptr[i * 2 + 1] = hexchars[hex];
				}
			}

			if (size_ptr) {
				u32 n;

				n = snprintf(size_str, sizeof(size_str), "%d",
					fw->size);
				memset(size_str + n, ' ', sizeof(size_str) - n);
				memcpy(size_ptr, size_str, 10);
			}

			response->data = buff;
		}

		upload_data_id = upload_id;
		upload_data = fw->data;
		upload_size = fw->size;

		return;
	}

	if (status == HTTP_CB_CLOSED) {
		file = fs_find_file("upload.html");

		if (file) {
			if (file->data != response->data)
				free((void *) response->data);
		}
	}
}

static void upload_bl_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	char *buff, *md5_ptr, *size_ptr, size_str[16];
	u8 md5_sum[16];
	struct httpd_form_value *fw;
	const struct fs_desc *file;
	int i;

	static char hexchars[] = "0123456789abcdef";

	if (status == HTTP_CB_NEW) {
		fw = httpd_request_find_value(request, "firmware");
		if (!fw) {
			response->info.code = 302;
			response->info.connection_close = 1;
			response->info.location = "/";
			return;
		}

		upload_type = 1;

		if (output_plain_file(response, "upload_bl.html")) {
			response->info.code = 500;
			return;
		}

		buff = malloc(response->size + 1);
		if (buff) {
			memcpy(buff, response->data, response->size);
			buff[response->size] = 0;

			md5_ptr = strstr(buff, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
			size_ptr = strstr(buff, "YYYYYYYYYY");

			if (md5_ptr) {
				md5((u8 *) fw->data, fw->size, md5_sum);
				for (i = 0; i < 16; i++) {
					u8 hex;

					hex = (md5_sum[i] >> 4) & 0xf;
					md5_ptr[i * 2] = hexchars[hex];
					hex = md5_sum[i] & 0xf;
					md5_ptr[i * 2 + 1] = hexchars[hex];
				}
			}

			if (size_ptr) {
				u32 n;

				n = snprintf(size_str, sizeof(size_str), "%d",
					fw->size);
				memset(size_str + n, ' ', sizeof(size_str) - n);
				memcpy(size_ptr, size_str, 10);
			}

			response->data = buff;
		}

		upload_data_id = upload_id;
		upload_data = fw->data;
		upload_size = fw->size;

		return;
	}

	if (status == HTTP_CB_CLOSED) {
		file = fs_find_file("upload_bl.html");

		if (file) {
			if (file->data != response->data)
				free((void *) response->data);
		}
	}
}

static void upload_factory_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	char *buff, *md5_ptr, *size_ptr, size_str[16];
	u8 md5_sum[16];
	struct httpd_form_value *fw;
	const struct fs_desc *file;
	int i;

	static char hexchars[] = "0123456789abcdef";

	if (status == HTTP_CB_NEW) {
		fw = httpd_request_find_value(request, "firmware");
		if (!fw) {
			response->info.code = 302;
			response->info.connection_close = 1;
			response->info.location = "/";
			return;
		}

		upload_type = 2;

		if (output_plain_file(response, "upload_factory.html")) {
			response->info.code = 500;
			return;
		}

		buff = malloc(response->size + 1);
		if (buff) {
			memcpy(buff, response->data, response->size);
			buff[response->size] = 0;

			md5_ptr = strstr(buff, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
			size_ptr = strstr(buff, "YYYYYYYYYY");

			if (md5_ptr) {
				md5((u8 *) fw->data, fw->size, md5_sum);
				for (i = 0; i < 16; i++) {
					u8 hex;

					hex = (md5_sum[i] >> 4) & 0xf;
					md5_ptr[i * 2] = hexchars[hex];
					hex = md5_sum[i] & 0xf;
					md5_ptr[i * 2 + 1] = hexchars[hex];
				}
			}

			if (size_ptr) {
				u32 n;

				n = snprintf(size_str, sizeof(size_str), "%d",
					fw->size);
				memset(size_str + n, ' ', sizeof(size_str) - n);
				memcpy(size_ptr, size_str, 10);
			}

			response->data = buff;
		}

		upload_data_id = upload_id;
		upload_data = fw->data;
		upload_size = fw->size;

		return;
	}

	if (status == HTTP_CB_CLOSED) {
		file = fs_find_file("upload_factory.html");

		if (file) {
			if (file->data != response->data)
				free((void *) response->data);
		}
	}
}

static void flashing_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW)
		output_plain_file(response, "flashing.html");
}

static void flashing_bl_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW)
		output_plain_file(response, "flashing_bl.html");
}

static void flashing_factory_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW)
		output_plain_file(response, "flashing_factory.html");
}

struct flashing_status {
	char buf[4096];
	int ret;
	int body_sent;
};

static void result_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	const struct fs_desc *file;
	struct flashing_status *st;
	u32 size;

	if (status == HTTP_CB_NEW) {
		st = calloc(1, sizeof(*st));
		if (!st) {
			response->info.code = 500;
			return;
		}

		st->ret = -1;

		response->session_data = st;

		response->status = HTTP_RESP_CUSTOM;

		response->info.http_1_0 = 1;
		response->info.content_length = -1;
		response->info.connection_close = 1;
		response->info.content_type = "text/html";
		response->info.code = 200;

		size = http_make_response_header(&response->info,
			st->buf, sizeof(st->buf));

		response->data = st->buf;
		response->size = size;

		return;
	}

	if (status == HTTP_CB_RESPONDING) {
		st = response->session_data;

		if (st->body_sent) {
			response->status = HTTP_RESP_NONE;
			return;
		}

		if (upload_data_id == upload_id) {
			if (upload_type == 0) {
				st->ret = write_firmware_failsafe((size_t) upload_data,
					upload_size);
			} else if (upload_type == 1) {
				st->ret = write_bootloader_failsafe((size_t) upload_data,
					upload_size);
			} else if (upload_type == 2) {
				st->ret = write_factory_failsafe((size_t) upload_data,
					upload_size);
			} else if (upload_type == 3) {
				st->ret = erase_nvram_failsafe();
			}
		}

		/* invalidate upload identifier */
		upload_data_id = rand();

		if (!st->ret)
			file = fs_find_file("success.html");
		else
			file = fs_find_file("fail.html");

		if (!file) {
			if (!st->ret)
				response->data = "Upgrade completed!";
			else
				response->data = "Upgrade failed!";
			response->size = strlen(response->data);
			return;
		}

		response->data = file->data;
		response->size = file->size;

		st->body_sent = 1;

		return;
	}

	if (status == HTTP_CB_CLOSED) {
		st = response->session_data;

		upgrade_success = !st->ret;

		free(response->session_data);
		
		if (upgrade_success)
			tcp_close_all_conn();
	}
}

static void style_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW) {
		output_plain_file(response, "style.css");
		response->info.content_type = "text/css";
	}
}

static void erase_nvram_html_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW)
		output_plain_file(response, "erase_nvram.html");
}

static void erase_select_html_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW)
		output_plain_file(response, "erase_select.html");
}

static void not_found_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW) {
		output_plain_file(response, "404.html");
		response->info.code = 404;
	}
}

static void reboot_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	if (status == HTTP_CB_NEW) {
		response->info.code = 200;
		response->info.connection_close = 1;
		response->info.content_type = "text/html";
		response->data = "<html><body><p>Rebooting now...</p></body></html>";
		response->size = strlen(response->data);
		response->status = HTTP_RESP_STD;
	}

	if (status == HTTP_CB_CLOSED) {
		tcp_close_all_conn();
		upgrade_success = 1;
	}
}

static void erase_nvram_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response)
{
	struct flashing_status *st;
	u32 size;

	if (status == HTTP_CB_NEW) {
		st = calloc(1, sizeof(*st));
		if (!st) {
			response->info.code = 500;
			return;
		}

		st->ret = erase_nvram_failsafe();

		response->session_data = st;

		response->status = HTTP_RESP_CUSTOM;

		response->info.http_1_0 = 1;
		response->info.content_length = -1;
		response->info.connection_close = 1;
		response->info.content_type = "text/html";
		response->info.code = 200;

		size = http_make_response_header(&response->info,
			st->buf, sizeof(st->buf));

		response->data = st->buf;
		response->size = size;

		return;
	}

	if (status == HTTP_CB_RESPONDING) {
		st = response->session_data;

		if (st->body_sent) {
			response->status = HTTP_RESP_NONE;
			return;
		}

		if (!st->ret)
			response->data = "NVRAM erase completed!";
		else
			response->data = "NVRAM erase failed!";

		response->size = strlen(response->data);
		st->body_sent = 1;

		return;
	}

	if (status == HTTP_CB_CLOSED) {
		free(response->session_data);
	}
}

/*
 * Generic erase partition handler.
 * IMPORTANT: perform the blocking erase in HTTP_CB_NEW (before sending response
 * header) so that we never race with HTTP_CB_CLOSED freeing the session_data.
 * This mirrors the working erase_nvram_handler() timing pattern.
 */
static void do_erase_partition_handler(enum httpd_uri_handler_status status,
	struct httpd_request *request,
	struct httpd_response *response,
	int(*erase_fn)(void),
	const char *name)
{
	struct flashing_status *st;
	u32 size;

	if (status == HTTP_CB_NEW) {
		st = calloc(1, sizeof(*st));
		if (!st) {
			response->info.code = 500;
			return;
		}

		printf("Web erase: erasing %s partition...\n", name);
		st->ret = erase_fn();
		printf("Web erase: %s %s (ret=%d)\n", name,
		       st->ret ? "FAILED" : "succeeded", st->ret);

		response->session_data = st;

		response->status = HTTP_RESP_CUSTOM;

		response->info.http_1_0 = 1;
		response->info.content_length = -1;
		response->info.connection_close = 1;
		response->info.content_type = "text/html";
		response->info.code = 200;

		size = http_make_response_header(&response->info,
			st->buf, sizeof(st->buf));

		response->data = st->buf;
		response->size = size;

		return;
	}

	if (status == HTTP_CB_RESPONDING) {
		st = response->session_data;

		if (st->body_sent) {
			response->status = HTTP_RESP_NONE;
			return;
		}

		/*
		 * Build the response body directly inside st->buf.
		 * This buffer has 4096 bytes and is allocated with calloc(),
		 * so it stays valid until HTTP_CB_CLOSED frees it.
		 * DO NOT use stack-local char arrays here: the response callback
		 * reads response->data after this function returns, by which time
		 * stack frames have been reclaimed and the pointer is dangling.
		 */
		if (!st->ret) {
			snprintf(st->buf, sizeof(st->buf),
				"<html><head>"
				"<meta http-equiv=\"refresh\" content=\"3;url=/erase_select.html\">"
				"</head><body>"
				"<p style=\"color:#060;\"><strong>%s erase completed!</strong></p>"
				"<p><a href=\"/erase_select.html\">Back to erase menu</a> | "
				"<a href=\"/\">Back to main menu</a></p>"
				"<p><a href=\"/reboot\"><strong>Reboot device</strong></a> "
				"(auto redirect in 3 seconds)</p>"
				"</body></html>",
				name);
		} else {
			snprintf(st->buf, sizeof(st->buf),
				"<html><body>"
				"<p style=\"color:#c00;\"><strong>%s erase failed! (ret=%d)</strong></p>"
				"<p>Please check the serial console log for details.</p>"
				"<p><a href=\"/erase_select.html\">Back to erase menu</a> | "
				"<a href=\"/\">Back to main menu</a></p>"
				"</body></html>",
				name, st->ret);
		}

		response->data = st->buf;
		response->size = strlen(st->buf);
		st->body_sent = 1;

		return;
	}

	if (status == HTTP_CB_CLOSED) {
		free(response->session_data);
	}
}

/* Wrapper for "erase all three" — calls each erase function sequentially */
static int erase_all_partitions_failsafe(void)
{
	int r, ret = 0;

	r = erase_nvram_failsafe_ex();
	if (r) ret = r;
	r = erase_factory_failsafe();
	if (r) ret = r;
	r = erase_factory2_failsafe();
	if (r) ret = r;

	return ret;
}

static void do_erase_nvram_handler(enum httpd_uri_handler_status s,
	struct httpd_request *req, struct httpd_response *rsp)
{
	do_erase_partition_handler(s, req, rsp, erase_nvram_failsafe_ex, "NVRAM");
}

static void do_erase_factory_handler(enum httpd_uri_handler_status s,
	struct httpd_request *req, struct httpd_response *rsp)
{
	do_erase_partition_handler(s, req, rsp, erase_factory_failsafe, "Factory");
}

static void do_erase_factory2_handler(enum httpd_uri_handler_status s,
	struct httpd_request *req, struct httpd_response *rsp)
{
	do_erase_partition_handler(s, req, rsp, erase_factory2_failsafe, "Factory2");
}

static void do_erase_all_handler(enum httpd_uri_handler_status s,
	struct httpd_request *req, struct httpd_response *rsp)
{
	do_erase_partition_handler(s, req, rsp, erase_all_partitions_failsafe,
				   "NVRAM+Factory+Factory2 (All)");
}

int start_web_failsafe(void)
{
	struct httpd_instance *inst;
#if defined(CONFIG_MTK_DHCPD)
	int dhcp_ret;
#endif

	/*
	 * Start DHCP server BEFORE httpd starts accepting connections, so that
	 * directly-connected PCs can obtain a 192.168.1.x address without any
	 * manual static-IP configuration.
	 *
	 * CRITICAL GUARANTEE AGAINST BRICKING:
	 *   mtk_dhcpd_start() is ONLY ever reached from start_web_failsafe(),
	 *   which is entered via the "httpd" U-Boot command / run_command() /
	 *   user-triggered failsafe entry point. It is NEVER attached to an
	 *   early-init hook such as board_init_f, last_stage_init, or
	 *   board_early_init_r. This guarantees that eth_init(), net_init(),
	 *   DDR/clock bring-up, and packet buffers (net_tx_packet/net_rx_packet)
	 *   are all fully set up before we call net_set_udp_handler() / touch
	 *   any network hardware.
	 *   The previous boot-time brick was NOT caused by mtk_dhcpd.c itself,
	 *   but by an "extern unsigned long DETECT(void) + run_command('httpd')"
	 *   block that got mistakenly inserted into last_stage_init() in
	 *   mt7621_nand_rfb.c. DETECT() did not exist, so the call jumped to
	 *   an unmapped address and halted the CPU before even the U-Boot
	 *   banner could print. The mt7621_nand_rfb.c file is LEFT UNTOUCHED in
	 *   this re-enable.
	 */
#if defined(CONFIG_MTK_DHCPD)
	dhcp_ret = mtk_dhcpd_start();
	if (dhcp_ret)
		printf("Warning: DHCP server failed to start (ret=%d). "
		       "Configure PC static IP 192.168.1.x manually.\n",
		       dhcp_ret);
	else
		printf("DHCP server started: pool 192.168.1.x, "
		       "gateway/dns = 192.168.1.1, lease ~3600s\n");
#endif

	inst = httpd_find_instance(80);
	if (inst)
		httpd_free_instance(inst);

	inst = httpd_create_instance(80);
	if (!inst) {
		printf("Error: failed to create HTTP instance on port 80\n");
		return -1;
	}

	httpd_register_uri_handler(inst, "/", &index_handler, NULL);
	httpd_register_uri_handler(inst, "/cgi-bin/luci", &index_handler, NULL);
	httpd_register_uri_handler(inst, "/upload", &upload_handler, NULL);
	httpd_register_uri_handler(inst, "/upload_bl", &upload_bl_handler, NULL);
	httpd_register_uri_handler(inst, "/upload_factory", &upload_factory_handler, NULL);
	httpd_register_uri_handler(inst, "/flashing", &flashing_handler, NULL);
	httpd_register_uri_handler(inst, "/flashing_bl", &flashing_bl_handler, NULL);
	httpd_register_uri_handler(inst, "/flashing_factory", &flashing_factory_handler, NULL);
	httpd_register_uri_handler(inst, "/result", &result_handler, NULL);
	httpd_register_uri_handler(inst, "/reboot", &reboot_handler, NULL);
	httpd_register_uri_handler(inst, "/erase_nvram", &erase_nvram_handler, NULL);
	httpd_register_uri_handler(inst, "/erase_nvram.html", &erase_nvram_html_handler, NULL);
	httpd_register_uri_handler(inst, "/erase_select", &erase_select_html_handler, NULL);
	httpd_register_uri_handler(inst, "/erase_select.html", &erase_select_html_handler, NULL);
	/* Independent erase handlers (blocking erase in HTTP_CB_NEW, safe) */
	httpd_register_uri_handler(inst, "/do_erase_nvram", &do_erase_nvram_handler, NULL);
	httpd_register_uri_handler(inst, "/do_erase_factory", &do_erase_factory_handler, NULL);
	httpd_register_uri_handler(inst, "/do_erase_factory2", &do_erase_factory2_handler, NULL);
	httpd_register_uri_handler(inst, "/do_erase_all", &do_erase_all_handler, NULL);
	httpd_register_uri_handler(inst, "/style.css", &style_handler, NULL);
	httpd_register_uri_handler(inst, "", &not_found_handler, NULL);

	/*
	 * A single net_loop(TCP) invocation drives BOTH the HTTP server on
	 * TCP:80 AND the DHCP server on UDP:67/68 in parallel. The dispatch
	 * switch on ip->ip_p inside net/net.c routes IPPROTO_TCP -> receive_tcp
	 * (for HTTP) and IPPROTO_UDP -> *udp_packet_handler (into
	 * dhcpd_udp_handler, which was installed by mtk_dhcpd_start() above).
	 * DHCP OFFER/ACK packets are transmitted synchronously from the UDP
	 * handler using net_set_udp_header() + net_send_packet(), independent
	 * of the TCP state machine.
	 */
	net_loop(TCP);

	return 0;
}

static int do_httpd(cmd_tbl_t *cmdtp, int flag, int argc,
	char *const argv[])
{
	int ret;

	printf("\nWeb failsafe UI started\n");
	
	ret = start_web_failsafe();

	if (upgrade_success)
		do_reset(NULL, 0, 0, NULL);

	return ret;
}

U_BOOT_CMD(httpd, 1, 0, do_httpd,
	"Start failsafe HTTP server", ""
);
