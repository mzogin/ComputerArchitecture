writeint:
	lodd on:
	stod 4095
	lodd neg1:		; -1 will mark end of result when printing
	push
	lodd mask:
	push
encode: lodd sum:
	jzer print:		; when quotient is zero, we can print
	lodd mask:		; load 10 and push on stack
	push
	lodd sum:		; load sum and push on stack
	push
	div			; divide sum by 10
	pop			; pop quotient into ac
	stod sum:		; store quotient in sum
	pop			; pop remainder(R) into ac
	insp 2			
	addd numoff:		; add 48 to R to convert to ascii
	push			; push it onto stack
	jump encode:		; loop back for rest of number
print:  pop			; pop first ascii char into ac
	jneg done:		; when we reach the -1 in stack, thats the end	
	stod 4094
	call xbsywt:
	jump print:
done:
	loco 0
        retn
