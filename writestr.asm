writestr:
        pshi			; gets first 2 chars on stack
        addd c1:		; add 1 to ptr to get next 2 chars on next pass
        stod pstr1:		; save the new ptr
        pop			; pop the value off stack (first 2 chars)
        jzer crnl:		; check for zero (end of string + even # of chars)
        stod 4094		; take ac value(2 ascii codes) and send to transmitter
        push			; put it on stack
        subd c255:		; subtract all 1s 
        jneg crnl:		; check if negative (if upper byte is all zeros)
        call sb:		; if upper byte has meaning full ascii code, swap bytes
        insp 1			; get arg off stack
        push			; push reversed chars on stack
        call xbsywt:		; returns when transmit is ready 
        pop			
        stod 4094		; send 16 bit val(high byte in low byte pos)to transmitter
        call xbsywt:		; prepare to transmit next set
        lodd pstr1:		; load ac with ptr
        jump writestr:		; next loop
crnl:   lodd cr:		; load ac with ascii for carriage return
        stod 4094		; store ito on transmitter
        call xbsywt:		
        lodd nl:		; load ac with newline ascii char
        stod 4094		; store to transmitter
        call xbsywt:
        retn

sb:     loco 8
loop1:  jzer finish:
        subd c1:
        stod lpcnt:
        lodl 1
        jneg add1:
        addl 1
        stol 1
        lodd lpcnt:
        jump loop1:
add1:   addl 1
        addd c1:
        stol 1
        lodd lpcnt:
        jump loop1:
finish: lodl 1
        retn
