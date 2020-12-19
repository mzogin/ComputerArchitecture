main:   lodd on:
        stod 4095
        call xbsywt:
        loco str1:
        push
        call writestr:
	insp 1
        call readint:
        stod binum1:
        call xbsywt:
        loco str1:
        push
        call writestr:
        insp 1
        call readint:
        stod binum2:
        lodd binum1:
        push
        lodd binum2:
        push
        call addints:
        insp 2
        stod sum:
        jpos prtsum:
        call xbsywt:
        loco str3:
        push
        call writestr:
        insp 1
        halt
prtsum: 
        call xbsywt:
        loco str2:
        push
        call writestr:
        insp 1
        call xbsywt:
        lodd sum:
        call writeint:
        halt
