.global _start
.data
dividend: .long 0

divisor: .long 0

quotient: .long 0

remaind: .long 0

buf: .long 0

.text
_start:

#  buf = divident;
movl dividend, %ecx
movl %ecx, buf

if_start:
#   if (buf < divisor)
    movl buf, %ecx
    cmpl %ecx, divisor
    # jump if divisor - buf <= 0
        jbe if_end
    movl $0, quotient
    # quotient = 0;
    movl buf, %ecx
    movl %ecx, remaind
    # remaind = buf;
    movl quotient, %eax
    movl remaind, %edx
    jmp done
if_end:

else_if_start:
#   else if (buf == divisor)
    movl buf, %ecx
    cmpl %ecx, divisor
    # jump if divisor - buf != 0
        jnz else_if_end
    movl $1, quotient
    # quotient = 1;
    movl $0, remaind
    # remaind = 0;
    movl quotient, %eax
    movl remaind, %edx
    jmp done
else_if_end:

else_start:
#   else
    movl $0, quotient
    # quotient = 0;
    movl $0, remaind
    # remaind = 0;
    movl $0, %edx
    # edx will hold i
    
    for_loop_start:
        # for (int i = 0; i < 32; ++i)
        cmpl $32, %edx
        # jump if i - 32 >= 0
            jae for_loop_end

        shll $1, quotient
        # quotient <<= 1;
        shll $1, remaind
        # remaind <<= 1;

        #if ((buf & 0x80000000) != 0)
        movl buf, %ebx
        if_start1:
            and $0x80000000, %ebx
            # buf & 0x80000000
            cmpl $0, %ebx
                # jump if ebx - 0 == 0
                jz if_end1
            add $1, remaind
            # remaind++;
        if_end1:

        if_start2:
        #   if (remaind >= divisor)
            movl remaind, %ecx
            cmpl %ecx, divisor
            # jump if divisor - remaind > 0
                ja if_end2
            movl divisor, %ecx
            sub %ecx, remaind
            # remaind -= divisor;
            add $1, quotient
            # quotient++;
        if_end2:

        shll $1, buf
        # buf <<= 1;
        add $1, %edx
        # i++;
        jmp for_loop_start
    for_loop_end:
    movl quotient, %eax
    movl remaind, %edx
else_end:

done:

movl %eax, %eax
