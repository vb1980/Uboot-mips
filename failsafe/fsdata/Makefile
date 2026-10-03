# SPDX-License-Identifier: GPL-2.0

# example:
# obj-y += XXXX.o <- object file to generate
# FILE_XXXX.o := YYYY.html <- actual html file to be embedded
# FSPATH_XXXX.o := ZZZ/WWW.EXT <- virtual fs path to be used inside u-boot

obj-y += index.o
FILE_index.o := index.html
FSPATH_index.o := index.html

obj-y += upload.o
FILE_upload.o := upload.html
FSPATH_upload.o := upload.html

obj-y += upload_bl.o
FILE_upload_bl.o := upload_bl.html
FSPATH_upload_bl.o := upload_bl.html

obj-y += upload_factory.o
FILE_upload_factory.o := upload_factory.html
FSPATH_upload_factory.o := upload_factory.html

obj-y += flashing.o
FILE_flashing.o := flashing.html
FSPATH_flashing.o := flashing.html

obj-y += flashing_bl.o
FILE_flashing_bl.o := flashing_bl.html
FSPATH_flashing_bl.o := flashing_bl.html

obj-y += flashing_factory.o
FILE_flashing_factory.o := flashing_factory.html
FSPATH_flashing_factory.o := flashing_factory.html

obj-y += erase_nvram.o
FILE_erase_nvram.o := erase_nvram.html
FSPATH_erase_nvram.o := erase_nvram.html

obj-y += flashing_nvram.o
FILE_flashing_nvram.o := flashing_nvram.html
FSPATH_flashing_nvram.o := flashing_nvram.html

obj-y += success.o
FILE_success.o := success.html
FSPATH_success.o := success.html

obj-y += fail.o
FILE_fail.o := fail.html
FSPATH_fail.o := fail.html

obj-y += 404.o
FILE_404.o := 404.html
FSPATH_404.o := 404.html

obj-y += style.o
FILE_style.o := style.css
FSPATH_style.o := style.css

obj-y += erase_select.o
FILE_erase_select.o := erase_select.html
FSPATH_erase_select.o := erase_select.html

obj-y += flashing_erase.o
FILE_flashing_erase.o := flashing_erase.html
FSPATH_flashing_erase.o := flashing_erase.html

# customized build rules
strip_path = $(subst /,_,$(subst -,_,$(subst .,_,$(1))))

quiet_cmd_as_o_html = AS      $@
cmd_as_o_html = $(CC) $(a_flags) -c -o $@ \
		-DDATA_SECT_NAME="\".rodata.fsdata.$(call strip_path,$(FSPATH_$(@F)))\"" \
		-DDATA_OBJ_FILE="\"$(obj)/$(FILE_$(@F))\"" \
		-DDATA_OBJ_NAME="fsdata_$(call strip_path,$(FSPATH_$(@F)))" \
		-DDATA_SIZE_NAME="fsdata_size_$(call strip_path,$(FSPATH_$(@F)))" \
		-DDATA_SECT_FSPATH_NAME="\".rodata.fsdata.path.$(call strip_path,$(FSPATH_$(@F)))\"" \
		-DDATA_OBJ_FSPATH_NAME="fsdata_path_$(call strip_path,$(FSPATH_$(@F)))" \
		-DDATA_OBJ_FSPATH="\"$(FSPATH_$(@F))\"" \
		-DUBOOT_LIST_SECT_NAME="\".u_boot_list_2_fs_2_$(call strip_path,$(FSPATH_$(@F)))\"" \
		-DUBOOT_LIST_NAME="fsdata_ulist_$(call strip_path,$(FSPATH_$(@F)))" \
		$(src)/embed.S

$(obj)/%.o: $(src)/%.html FORCE
	$(call cmd,force_checksrc)
	$(call if_changed_dep,as_o_html)

$(obj)/%.o: $(src)/%.css FORCE
	$(call cmd,force_checksrc)
	$(call if_changed_dep,as_o_html)
