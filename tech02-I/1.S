    .global sum
    .text

end:
    ret

sum:
    cmp x1, 0
    ble end
    ldr x3, [x2]
    add x2, x2, 4
    add x0, x0, x3
    sub x1, x1, 1
    b sum