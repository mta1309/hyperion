# This test file was generated from offline assembler source
# by bldhtc.rexx 16 Jan 2016 12:11:11
# Treat as object code.  That is, modifications will be lost.
# assemble and listing files are provided for information only.
*Testcase sigp
numcpu 1
sysclear
archmode z
r    1A0=00000001800000000000000000000200
r    1D0=0002000180000000FFFFFFFFDEADDEAD
r    200=B212022048200220AE020005B2B20210
r    210=00020000000000000000000000000000
runtest .1
psw
runtest start .1
psw
*Done
