; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=aarch64-none-linux-gnu -global-isel -global-isel-abort=0 | FileCheck %s

define i32 @f(i64 %0) {
; CHECK-LABEL: f:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov x8, #-7378697629483820647 // =0x9999999999999999
; CHECK-NEXT:    mov w9, w0
; CHECK-NEXT:    mov w10, #10 // =0xa
; CHECK-NEXT:    eor x8, x8, #0x8000000000000003
; CHECK-NEXT:    umulh x8, x9, x8
; CHECK-NEXT:    msub x0, x8, x10, x9
; CHECK-NEXT:    // kill: def $w0 killed $w0 killed $x0
; CHECK-NEXT:    ret
  %2 = trunc i64 %0 to i32
  %3 = freeze i32 %2
  %4 = zext i32 %3 to i64
  %5 = urem i64 %4, 10
  %6 = trunc i64 %5 to i32
  ret i32 %6
}
