CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 insertion_sort tst_sub_sbc_rsb tst_add_adc tst_and_eor

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
insertion_sort_SOURCES=insertion_sort.c

tst_and_eor_SOURCES=tst_and_eor.s
tst_add_adc_SOURCES=tst_add_adc.s
tst_sub_sbc_rsb_SOURCES=tst_sub_sbc_rsb.s

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
