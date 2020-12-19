addints: 
	lodd binum1:		; load first num in ac	
	stod sum:		; store it in sum
	lodd binum2:		; load second num in ac
	addd sum:		; add it with other num
	stod sum:		; store into sum
	jneg ovrflow:		; negative result from addition = overflow
	retn
ovrflow:
        lodd neg1:              ; keep the return code as -1 in ac
	retn
