// NOTE: Assertions have been autogenerated by utils/update_mc_test_checks.py UTC_ARGS: --version 5
// RUN: llvm-mc -triple=amdgcn -mcpu=gfx1200 -show-encoding %s | FileCheck -check-prefix=GFX12 %s

lds_direct_load v0
// GFX12: ds_direct_load v0 wait_va_vdst:0 wait_vm_vsrc:0 ; encoding: [0x00,0x00,0x10,0xce]

lds_param_load v0, attr0.x
// GFX12: ds_param_load v0, attr0.x wait_va_vdst:0 wait_vm_vsrc:0 ; encoding: [0x00,0x00,0x00,0xce]
