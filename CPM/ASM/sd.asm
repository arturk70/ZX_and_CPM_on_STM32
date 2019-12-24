;
;			SD.ASM
;		  (revised 2/6/81)OC
;
;		SUPER DIRECTORY	PROGRAM
;		  by Bruce R. Ratoff
;
;Displays the directory	of a CP/M disk,	sorted alphabetically,
;with the file size in K, rounded to the nearest CP/M block size.
;
;This latest variation on a common theme will automatically adjust
;itself	for any	block size and directory length	under CP/M 1.4 or 2.x
;or MP/M (any version).	 If the	screen fills, program will pause until
;a key is struck (see NPL and LPS equates below).  Total space used
;and number of files are printed at end.
;
;Command: SD FILENAME.FILETYPE or just SD
;
;Allows	'*' or '?' type	specifications.	 Drive name may	also be
;specified.  Ignores "SYS" files unless	SOPT is	TRUE and 'S' option
;is given (i.e., SD *.*	S will print all files).
;
;
;
;2/6/81	  CHANGED SORT TO HAVE ODD GAP(K+P SAY ITS FASTER)
;
;1/6/80	  CHANGED SORT FROM BUBBLE SORT	TO SHELL SHORT
;	  FOR FASTER SPEED.
;
;12/24/80 CHANGED BIOS CONOUT TO BDOS CONOUT
;	  TO ALLOW PRINTING OF DIRECTORY WITH
;	  CONTROL P.
;	  ALSO ADDED PRINT OF REMAINING	SPACE 
;	  EVEN IF FILE NOT FOUND.(STEVE	NOSSEN)
;
;12/15/80 Added	space suppression when printing	file
;	  totals.  (KBP)
;
;12/14/80 Added	logic to print space remaining on disk.
;	  Changed ^C test so that interrupting character is
;	  not echoed (makes remote use cleaner).  (BRR)
;
;12/02/80 Fixed	bug in print routine which compared last file
;	  against garbage before printing. (BRR)
;
;11/29/80 Changed to allow printing 4 file names. (Ben Bronson
;	  and Keith Petersen)
;
;11/22/80 Fixed	bug in handling	>256 files.  Changed abort test	in
;	  print	routine	to only	abort on control-c.  (brr)
;
;Based on 'DIRS' by Keith Petersen, W8SDZ
;
;Set 'RMAC' TRUE to assemble with relocating assembler (requires
;link with PAGE	0 equates in separate file).
;
FALSE	EQU	0		;DEFINE	LOGICAL	FALSE
TRUE	EQU	NOT FALSE	;DEFINE	LOGICAL	TRUE
;
ALTCPM	EQU	FALSE	;PUT TRUE HERE FOR H8 OR TRS-80
RMAC	EQU	FALSE	;PUT TRUE HERE FOR ASSEMBLY BY RMAC
SOPT	EQU	TRUE   ;PUT TRUE TO ALLOW 'DIR *.* S' FORM
WIDE	EQU	FALSE	;PUT TRUE TO ALLOW 4 NAMES ACROSS
;
IF	ALTCPM
BASE	EQU	4200H
TPA	EQU	4300H
ENDIF
;
IF	RMAC
    EXTRN	BASE,FCB,BDOS	;MAKE BASE EXTERNAL
ENDIF
;
IF	(NOT ALTCPM) AND (NOT RMAC)
BASE	EQU	$	;WILL DEFAULT TO 0 (OR 100H WITH MAC +R	OPTION)
TPA	EQU	100H
ENDIF
;
IF	NOT RMAC
FCB	EQU	BASE+5CH
BDOS	EQU	BASE+5
ENDIF
;
IF	WIDE
NPL	EQU	4	;NUMBER	OF NAMES PER LINE
ENDIF
;
IF	NOT WIDE
NPL	EQU	2	;NUMBER	OF NAMES PER LINE
ENDIF
;
LPS	EQU	23	;NUMBER	OF LINES PER SCREEN
DELIM	EQU	':'	;FENCE (DELIMITER) CHARACTER
;
IF	NOT RMAC
    ORG	TPA
ENDIF
;
;Save the stack
START:	LXI	H,0
    DAD	SP	;H=STACK
    SHLD	STACK	 ;SAVE IT
    LXI	SP,STACK ;GET NEW STACK
;
IF	SOPT
    LDA	FCB+17	 ;SAVE S OPTION	FLAG
    STA	SOPFLG	 ;(BLANK OR LETTER S)
ENDIF
;
    SUB	A
    STA	USERNO	 ;DEFAULT TO USER 0
    STA	LINCNT	 ;CLEAR	COUNT OF LINES ON SCREEN
    MVI	C,12
    CALL	BDOS	 ;CHECK	CP/M VERSION
    SHLD	VERFLG	 ;LO ORD >0 IF 2.X, HI ORD>0 IF	MP/M
    MOV	A,L	 ;2.X?
    ORA	A
    JZ	CHKDRV	 ;SKIP USER NUMBER STUFF IF 1.4
    MVI	E,0FFH
    MVI	C,CURUSR ;INTERROGATE USER NUMBER
    CALL	BDOS
    STA	USERNO
    LDA	MPMFLG	 ;MP/M?
    ORA	A	;IF SO,	TYPE HEADING LINE
    JZ	CHKDRV	 ; ELSE	SKIP IT
    LXI	D,USRMSG ;DISPLAY IT
    MVI	C,PRINT
    CALL	BDOS	 ;FIRST	PART OF	MESSAGE
    LDA	USERNO
    CPI	10	;IF USER NO. > 9 PRINT LEADING 1
    JC	DUX
    MVI	A,'1'
    CALL	TYPE
    LDA	USERNO	 ;PRINT	LOW DIGIT OF USER NO.
    SUI	10
;
DUX:	ADI	'0'
    CALL	TYPE
    LXI	D,USRMS2 ;PRINT	TAIL OF	MESSAGE
    MVI	C,PRINT
    CALL	BDOS
    MVI	A,1
    STA	LINCNT	;WE USED A LINE
;
CHKDRV:	LXI	H,FCB
    MOV	A,M	;GET DRIVE NAME
    ORA	A	;ANY SPECIFIED?
    JNZ	START2	;YES SKIP NEXT ROUTINE
    MVI	C,CURDSK
    CALL	BDOS	;GET CURRENT DISK NR
    INR	A	;MAKE A:=1
    STA	FCB
;
START2:	ADI	'A'-1	;MAKE IT PRINTABLE
    STA	DRNAM	;SAVE FOR LATER
    LXI	H,FCB+1	;POINT TO NAME
    MOV	A,M	;ANY SPECIFIED?
    CPI	' '
    JNZ	GOTFCB
;No FCB	- make FCB all '?'
    MVI	B,11	;FN+FT COUNT
;
QLOOP:	MVI	M,'?'	;STORE '?' IN FCB
    INX	H
    DCR	B
    JNZ	QLOOP
;
GOTFCB:	MVI	A,'?'	;FORCE WILD EXTENT
    STA	FCB+12
    LDA	FCB	;CHECK FOR EXPLICIT DRIVE
    DCR	A
    MOV	E,A	;SELECT	SPECIFIED DRIVE
    MVI	C,SELDSK
    CALL	BDOS
    SUB	A	;ZAP DRIVE NO. IN FCB SO SELECTED DRIVE	IS USED
    STA	FCB
    LDA	VERFLG	;CHECK VERS.
    ORA	A
    JZ	V14	;PRE-2.X...GET PARAMS THE 1.4 WAY
;
    MVI	C,CURDPB;IT'S 2.X OR MP/M...REQUEST DPB
    CALL	BDOS
    INX	H
    INX	H
    MOV	A,M	;GET BLOCK SHIFT
    STA	BLKSHF
    INX	H	;BUMP TO BLOCK MASK
    MOV	A,M
    STA	BLKMSK	;GET IT
    INX	H
    INX	H
    MOV	E,M	;GET MAX BLOCK #
    INX	H
    MOV	D,M
    XCHG
    SHLD	BLKMAX	;SAVE IT
    XCHG
    INX	H
    MOV	E,M	;GET DIRECTORY SIZE
    INX	H
    MOV	D,M
    XCHG
    SHLD	DIRMAX	;SAVE IT
    JMP	SETTBL	;DONE...GO SET UP ORDER	TABLE
;
V14:	LHLD	BDOS+1	;GET PARAMS 1.4	STYLE
    MVI	L,3BH	;POINT TO DIRECTORY SIZE
    MOV	E,M	;GET IT
    MVI	D,0	;FORCE HI ORD TO 0
    PUSH	D	;SAVE FOR LATER
    INX	H	;POINT TO BLOCK	SHIFT
    MOV	A,M	;FETCH
    STA	BLKSHF	;SAVE
    INX	H	;POINT TO BLOCK	MASK
    MOV	A,M	;FETCH IT
    STA	BLKMSK	;AND SAVE IT
    INX	H
    MOV	E,M	;GET MAX. BLOCK	NO.
    MVI	D,0
    XCHG
    SHLD	BLKMAX	;SAVE IT
    POP	H	;RESTORE DIRECTORY SIZE
    SHLD	DIRMAX	;SAVE IT
;
SETTBL:	INX	H	;DIRECTORY SIZE	IS DIRMAX+1
    DAD	H	;DOUBLE	DIRECTORY SIZE
    LXI	D,ORDER	;TO GET	SIZE OF	ORDER TABLE
    DAD	D	;ALLOCATE ORDER	TABLE
    SHLD	TBLOC	;NAME TABLE BEGINS WHERE ORDER TABLE ENDS
    SHLD	NEXTT
    XCHG
    LHLD	BDOS+1	;MAKE SURE WE HAVE ROOM	TO CONTINUE
    MOV	A,E
    SUB	L
    MOV	A,D
    SBB	H
    JNC	OUTMEM
;
;Look up the FCB in the	directory
SFIRST:	MVI	C,FSRCHF ;GET 'SEARCH FIRST' FNC
    LXI	D,FCB
    CALL	BDOS	;READ FIRST
    INR	A	;WERE THERE ANY?
    JNZ	SOME	;GOT SOME
;
NONE:	LXI	D,FNF	;PREPARE MP/M ERROR MESSAGE
    LDA	MPMFLG
    ORA	A	;USE IT	IF REALLY MP/M
    JNZ	ERXIT1
    CALL	ERXIT	;ELSE USE CP/M ERROR MESSAGE
    DB	'NO FILE$'
FNF:	DB	'File not found.$'
;
USRMSG:	DB	'Directory for user $'
USRMS2:	DB	':',13,10,'$'
;
;Read more directory entries
MORDIR:	MVI	C,FSRCHN ;SEARCH NEXT
    LXI	D,FCB
    CALL	BDOS	;READ DIR ENTRY
    INR	A	;CHECK FOR END (0FFH)
    JZ	SPRINT	;NO MORE - SORT	& PRINT
;
;Point to directory entry 
SOME:	DCR	A	;UNDO PREV 'INR	A'
    ANI	3	;MAKE MODULUS 4
    ADD	A	;MULTIPLY...
    ADD	A	;..BY 32 BECAUSE
    ADD	A	;..EACH	DIRECTORY
    ADD	A	;..ENTRY IS 32
    ADD	A	;..BYTES LONG
    LXI	H,BASE+81H ;POINT TO BUFFER
	    ;(SKIP TO FN/FT)
    ADD	L	;POINT TO ENTRY
    ADI	9	;POINT TO SYS BYTE
    MOV	L,A	;SAVE (CAN'T CARRY TO H)
;
IF	SOPT
    LDA	SOPFLG	;DID USER REQUEST SYS FILES?
    CPI	'S'
    JZ	SYSFOK
ENDIF
;
    MOV	A,M	;GET SYS BYTE
    ANI	80H	;CHECK BIT 7
    JNZ	MORDIR	;SKIP THAT FILE
;
SYSFOK:	MOV	A,L	;GO BACK NOW
    SUI	10	;BACK TO USER NUMBER (ALLOC FLAG)
    MOV	L,A	;HL POINTS TO ENTRY NOW
;
    LDA	USERNO	;GET CURRENT USER
    CMP	M
    JNZ	MORDIR	;IGNORE	IF DIFFERENT
    INX	H
;
;Move entry to table
    XCHG		;ENTRY TO DE
    LHLD	NEXTT	;NEXT TABLE ENTRY TO HL
    MVI	B,12	;ENTRY LENGTH (NAME, TYPE, EXTENT)
;
TMOVE:	LDAX	D	;GET ENTRY CHAR
    ANI	7FH	;REMOVE	ATTRIBUTES
    MOV	M,A	;STORE IN TABLE
    INX	D
    INX	H
    DCR	B	;MORE?
    JNZ	TMOVE
    INX	D
    INX	D	;POINT TO SECTOR COUNT
    LDAX	D	; GET IT
    MOV	M,A	;STORE IN TABLE
    INX	H
    SHLD	NEXTT	;SAVE UPDATED TABLE ADDR
    XCHG
    LHLD	COUNT	;GET PREV COUNT
    INX	H
    SHLD	COUNT
    LXI	H,13	;SIZE OF NEXT ENTRY
    DAD	D
    XCHG		;FUTURE	NEXTT IS IN DE
    LHLD	BDOS+1	;PICK UP TPA END
    MOV	A,E
    SUB	L	;COMPARE NEXTT-TPA END
    MOV	A,D
    SBB	H
    JC	MORDIR	;IF TPA	END>NEXTT THEN LOOP BACK FOR MORE
;
OUTMEM:	CALL	ERXIT
    DB	'Out of	memory.',13,10,'$'
;
;Sort and print
SPRINT:	LHLD	COUNT	;GET FILE NAME COUNT
    MOV	A,L
    ORA	H	;ANY FOUND?
    JZ	NONE	;NO, EXIT
    PUSH	H	;SAVE FILE COUNT
;Init the order	table
    LHLD	TBLOC	;GET START OF NAME TABLE
    XCHG		;INTO DE
    LXI	H,ORDER	;POINT TO ORDER	TABLE
    LXI	B,13	;ENTRY LENGTH
;
BLDORD:	MOV	M,E	;SAVE LO ORD ADDR
    INX	H
    MOV	M,D	;SAVE HI ORD ADDR
    INX	H
    XCHG		;TABLE ADDR TO HL
    DAD	B	;POINT TO NEXT ENTRY
    XCHG
    XTHL		;SAVE TBL ADDR,	FETCH LOOP COUNTER
    DCX	H	;COUNT DOWN LOOP
    MOV	A,L
    ORA	H	;MORE?
    XTHL		;(RESTORE TBL ADDR, SAVE COUNTER)
    JNZ	BLDORD	;..YES,	GO DO ANOTHER ONE
    POP	H	;CLEAN LOOP COUHTER OFF	STACK
    LHLD	COUNT	;GET COUNT
    SHLD	SCOUNT	;SAVE AS # TO SORT
    DCX	H	;ONLY 1	ENTRY?
    MOV	A,L
    ORA	H
    JZ	DONE	;..YES,	SO SKIP	SORT
;
; THIS SORT ROUTINE IS ADAPTED FROM SOFTWARE TOOLS
; BY KERNIGAN AND PLAUGHER.
;
SORT:	LHLD	SCOUNT	;NUMBER	OF ENTRIES
L0:	ORA	A	;CLEAR CARRY
    MOV	A,H	;GAP=GAP/2
    RAR
    MOV	H,A
    MOV	A,L
    RAR
    MOV	L,A
    ORA	H	;IS IT ZERO?
    JZ	DONE	;THEN NONE LEFT
    MOV	A,L	;MAKE GAP ODD
    ORI	01
    MOV	L,A
    SHLD	GAP
    INX	H	;I=GAP+1
L2:	SHLD	I
    XCHG
    LHLD	GAP
    MOV	A,E	;J=I-GAP
    SUB	L
    MOV	L,A
    MOV	A,D
    SBB	H
    MOV	H,A
L3:	SHLD	J
    XCHG
    LHLD	GAP	;JG=J+GAP
    DAD	D
    SHLD	JG
    MVI	A,12	;COMPARE 12 CHARS
    CALL	COMPARE	;COMPARE (J) AND (JG)
    JP	L5	;IF A(J)<=A(JG)
    LHLD	J
    XCHG
    LHLD	JG
    CALL	SWAP	;EXCH A(J) AND A(JG)
    LHLD	J	;J=J-GAP
    XCHG
    LHLD	GAP
    MOV	A,E
    SUB	L
    MOV	L,A
    MOV	A,D
    SBB	H
    MOV	H,A
    JM	L5	;IF J>0	GOTO L3
    ORA	L	;CHECK FOR ZERO
    JZ	L5
    JMP	L3
L5:	LHLD	SCOUNT	;FOR LATER
    XCHG
    LHLD	I	;I=I+1
    INX	H
    MOV	A,E	;IF I<=N GOTO L2
    SUB	L
    MOV	A,D
    SBB	H
    JP	L2
    LHLD	GAP
    JMP	L0
;
;Sort is all done - print entries
DONE:	LXI	H,ORDER
    SHLD	NEXTT
;
;Print an entry
IF	NOT WIDE
    CALL	DRPRNT	;PRINT DRIVE NAME
ENDIF
    MVI	C,NPL	;NR. OF	NAMES PER LINE
    LXI	H,0	;ZERO OUT
    SHLD	TOTSIZ	; TOTAL	K USED
    SHLD	TOTFIL	; AND TOTAL FILES
;
ENTRY:	LHLD	COUNT	; CHECK	COUNT OF REMAINING FILES
    DCX	H	;  1 LEFT?
    MOV	A,L	; IF ONLY 1 LEFT, SKIP COMPARE WITH NEXT
    ORA	H	;  (SINCE THERE	ISN'T ANY NEXT)
    JZ	OKPRNT
    PUSH	B
    MVI	C,0BH	;CHECK CONSOLE STATUS
    CALL	BDOS
    ORA	A
    JZ     NOBRK   ;NO, CONTINUE
    JMP	EXIT
;
NOBRK:	LHLD	NEXTT
    MVI	A,11
    CALL	COMPR	;DOES THIS ENTRY MATCH NEXT ONE?
    POP	B
    JNZ	OKPRNT	;NO, PRINT IT
    INX	H
    INX	H	;SKIP, SINCE HIGHEST EXTENT COMES LAST IN LIST
    SHLD	NEXTT
    LHLD	COUNT
    DCX	H
    SHLD	COUNT	;COUNT DOWN
    JMP	ENTRY	;GO GET	NEXT
;
GOHL:	PCHL		;KLUDGE	TO ALLOW CALL TO ADDRESS IN HL
;
OKPRNT:	
IF	NOT WIDE
    CALL	FENCE	;PRINT FENCE CHAR AND SPACE
ENDIF
    LHLD	NEXTT	;GET ORDER TABLE POINTER
    MOV	E,M	;GET LO	ADDR
    INX	H
    MOV	D,M	;GET HI	ADDR
    INX	H
    SHLD	NEXTT	;SAVE UPDATED TABLE POINTER
    XCHG		;TABLE ENTRY TO	HL
    MVI	B,8	;FILE NAME LENGTH
    CALL	TYPEIT	;TYPE FILENAME
    MVI	A,' '	;SPACE AFTER FN
    CALL	TYPE
    MVI	B,3	;GET THE FILETYPE
    CALL	TYPEIT
    MOV	E,M	;GET EXTENT #
    MVI	D,0
    INX	H
    MOV	A,M	;GET SECTOR COUNT OF LAST EXTENT
    XCHG
    DAD	H	;# OF EXTENTS TIMES 16K
    DAD	H
    DAD	H
    DAD	H
    XCHG		;SAVE IN DE
    LXI	H,BLKMSK
    ADD	M	;ROUND LAST EXTENT TO BLOCK SIZE
    RRC
    RRC		;CONVERT FROM SECTORS TO K
    RRC
    ANI	1FH
    MOV	L,A	;ADD TO	TOTAL K
    MVI	H,0
    DAD	D
    LDA	BLKMSK	;GET SECTORS/BLK-1
    RRC
    RRC		;CONVERT TO K/BLK
    RRC
    ANI	1FH
    CMA		;USE TO	FINISH ROUNDING
    ANA	L
    MOV	L,A
    XCHG		;SAVE FILE SIZE	IN DE
    LHLD	TOTSIZ
    DAD	D	;ADD TO	TOTAL USED
    SHLD	TOTSIZ
    LHLD	TOTFIL	;INCREMENT FILE	COUNT
    INX	H
    SHLD	TOTFIL
    XCHG		;GET BACK FILE SIZE
    CALL	DECPRT	; AND PRINT IT
    MVI	A,'k'	;FOLLOW	WITH K
    CALL	TYPE
;
IF	NOT WIDE
    CALL	SPACE
ENDIF
;
;See if	more entries
    LHLD	COUNT	;COUNT DOWN ENTRIES
    DCX	H
    MOV	A,L
    ORA	H
    JZ	PRTOTL	;IF OUT	OF FILES, PRINT	TOTALS
    SHLD	COUNT
    DCR	C	;ONE LESS ON THIS LINE
;
IF	WIDE
    PUSH	PSW
    CNZ	FENCE	;NO CR-LF NEEDED, DO FENCE
    POP	PSW
ENDIF
;
    CZ	CRLF	;CR-LF NEEDED
    JMP	ENTRY
;
;Print HL in decimal with leading zero suppression
DECPRT:	SUB	A	;CLEAR LEADING ZERO FLAG
    STA	LZFLG
    LXI	D,-1000	;PRINT 1000'S DIGIT
    CALL	DIGIT
    LXI	D,-100	;ETC
    CALL	DIGIT
    LXI	D,-10
    CALL	DIGIT
    MVI	A,'0'	;GET 1'S DIGIT
    ADD	L
    JMP	TYPE
;
DIGIT:	MVI	B,'0'	;START OFF WITH	ASCII 0
;
DIGLP:	PUSH	H	;SAVE CURRENT REMAINDER
    DAD	D	;SUBTRACT
    JNC	DIGEX	;QUIT ON OVERFLOW
    POP	PSW	;THROW AWAY REMAINDER
    INR	B	;BUMP DIGIT
    JMP	DIGLP	;LOOP BACK
;
DIGEX:	POP	H	;RESTORE POINTER
    MOV	A,B
    CPI	'0'	;ZERO DIGIT?
    JNZ	DIGNZ	;NO, TYPE IT
    LDA	LZFLG	;LEADING ZERO?
    ORA	A
    MVI	A,'0'
    JNZ	TYPE	;PRINT DIGIT
    LDA	SUPSPC	;GET SPACE SUPPRESSION FLAG
    ORA	A	;SEE IF	PRINTING FILE TOTALS
    RZ		;YES, DON'T GIVE LEADING SPACES
    JMP	SPACE	;LEADING ZERO...PRINT SPACE
;
DIGNZ:	STA	LZFLG	;SET LEADING ZERO FLAG SO NEXT ZERO PRINTS
    JMP	TYPE	;AND PRINT DIGIT
;
;Show total space and files used
PRTOTL:	XRA	A	;GET A ZERO TO...
    STA	SUPSPC	;SUPPRESS LEADING SPACES IN TOTALS
    CALL	CRLF	;NEW LINE (WITH	PAUSE IF NECESSARY)
;
IF	WIDE
    LXI	D,TOTMS1 ;PRINT	FIRST PART OF TOTAL MESSAGE
ENDIF
;
IF	NOT WIDE
    LXI	D,TOTMS1+1 ;PRINT FIRST	PART OF	TOTAL MESSAGE
ENDIF
;
    MVI	C,PRINT
    CALL	BDOS
    LHLD	TOTSIZ	;PRINT TOTAL K USED
    CALL	DECPRT
    LXI	D,TOTMS2;NEXT PART OF MESSAGE
    MVI	C,PRINT
    CALL	BDOS
    LHLD	TOTFIL	;PRINT COUNT OF	FILES
    CALL	DECPRT
    LXI	D,TOTMS3;TAIL OF MESSAGE
    MVI	C,PRINT
    CALL	BDOS
PRT1:	MVI	C,GALLOC	;GET ADDRESS OF	ALLOCATION VECTOR
    CALL	BDOS
    XCHG
    LHLD	BLKMAX	;GET ITS LENGTH
    INX	H
    LXI	B,0	;INIT BLOCK COUNT TO 0
;
GSPBYT:	PUSH	D	;SAVE ALLOC ADDRESS
    LDAX	D
    MVI	E,8	;SET TO	PROCESS	8 BLOCKS
;
GSPLUP:	RAL		;TEST BIT
    JC	NOTFRE
    INX	B
;
NOTFRE:	MOV	D,A	;SAVE BITS
    DCX	H	;COUNT DOWN BLOCKS
    MOV	A,L
    ORA	H
    JZ	ENDALC	;QUIT IF OUT OF	BLOCKS
    MOV	A,D	;RESTORE BITS
    DCR	E	;COUNT DOWN 8 BITS
    JNZ	GSPLUP	;DO ANOTHER BIT
    POP	D	;BUMP TO NEXT BYTE
    INX	D	;OF ALLOC. VECTOR
    JMP	GSPBYT	;PROCESS IT
;
ENDALC:	MOV	L,C	;COPY BLOCKS TO	HL
    MOV	H,B
    LDA	BLKSHF	;GET BLOCK SHIFT FACTOR
    SUI	3	;CONVERT FROM SECTORS TO K
    JZ	PRTFRE	;SKIP SHIFTS IF	1K BLOCKS
;
FREKLP:	DAD	H	;MULT BLKS BY K/BLK
    DCR	A
    JNZ	FREKLP
;
PRTFRE:	CALL	DECPRT	;PRINT K FREE
    LXI	D,TOTMS4
    MVI	C,PRINT
    CALL	BDOS
    JMP	EXIT	;ALL DONE...RETURN TO CP/M
;
DRNAM	EQU	$	;SAVE DRIVE NAME HERE
TOTMS1:	DB	' : Total of $'
TOTMS2: DB	'k in $'
TOTMS3: DB	' files with $'
TOTMS4: DB	'k space remaining.$'
;
FENCE:	
IF	WIDE
    CALL	SPACE
ENDIF
    MVI	A,DELIM	;FENCE CHARACTER
    CALL	TYPE	;PRINT IT, FALL	INTO SPACE
;
SPACE:	MVI	A,' '
;
;Type character	in A
TYPE:	PUSH	B
    PUSH	D
    PUSH	H
    MOV	E,A
    MVI	C,2		;BDOS CONOUT
    CALL	BDOS
    POP	H
    POP	D
    POP	B
    RET
;
TYPEIT:	MOV	A,M
    CALL	TYPE
    INX	H
    DCR	B
    JNZ	TYPEIT
    RET
;
;Fetch character from console (without echo)
CINPUT:	LHLD	BASE+1
    MVI	L,9
    CALL	GOHL
    ANI	7FH
    RET
;
CRLF:	LDA	LINCNT	;CHECK FOR END OF SCREEN
    INR	A
    CPI	LPS
    JC	NOTEOS	;SKIP MESSAGE IF MORE LINES LEFT ON SCREEN
    LXI	D,EOSMSG;SAY WE'RE PAUSING FOR INPUT
    MVI	C,PRINT
    CALL	BDOS
    CALL	CINPUT	;WAIT FOR CHAR.
    SUB	A	;SET UP	TO ZERO	LINE COUNT
;
NOTEOS:	STA	LINCNT	;SAVE NEW LINE COUNT
    MVI	A,13	;PRINT CR
    CALL	TYPE
    MVI	A,10	;LF
    CALL	TYPE
;
IF	NOT WIDE
    CALL	DRPRNT	;DRIVE NAME
ENDIF
;
    MVI	C,NPL	;RESET NUMBER OF NAMES PER LINE
    RET
;
EOSMSG:	DB	13,10,'(Strike any key to continue)$'
;
IF	NOT WIDE
DRPRNT:	LDA	DRNAM
    JMP	TYPE
ENDIF
;
;Compare routine for sort
COMPR:	PUSH	H	;SAVE TABLE ADDR
    MOV	E,M	;LOAD LO
    INX	H
    MOV	D,M	;LOAD HI
    INX	H
    MOV	C,M
    INX	H
    MOV	B,M
;BC, DE	now point to entries to	be compared
    XCHG
    MOV	E,A	;GET COUNT
;
CMPLP:	LDAX	B
    CMP	M
    INX	H
    INX	B
    JNZ	NOTEQL	;QUIT ON MISMATCH
    DCR	E	;OR END	OF COUNT
    JNZ	CMPLP
;
NOTEQL:	POP	H
    RET		;COND CODE TELLS ALL
;
;Swap entries in the order table
SWAP:	LXI	B,ORDER-2	;TABLE BASE
    DAD	H	;*2
    DAD	B	;+ BASE
    XCHG
    DAD	H	;*2
    DAD	B	;+ BASE
    MOV	C,M
    LDAX	D
    XCHG
    MOV	M,C
    STAX	D
    INX	H
    INX	D
    MOV	C,M
    LDAX	D
    XCHG
    MOV	M,C
    STAX	D
    RET
;
;
; NEW COMPARE ROUTINE
;
COMPARE:	LXI	B,ORDER-2
    DAD	H
    DAD	B
    XCHG
    DAD	H
    DAD	B
    XCHG
    MOV	C,M
    INX	H
    MOV	B,M
    XCHG
    MOV	E,M
    INX	H
    MOV	D,M
    XCHG
    MOV	E,A	;COUNT
CMPLPE:	LDAX	B
    CMP	M
    INX	B
    INX	H
    RNZ
    DCR	E
    JNZ	CMPLPE
    RET
;
;Error exit
ERXIT:	POP	D	;GET MSG
;
ERXIT1:	MVI	C,PRINT
;
CALLB:	CALL	BDOS	;PERFORM REQUESTED FUNCTION
    CALL	CRLF
    LXI	D,TOTMS1
    MVI	C,PRINT
    CALL	BDOS
    XRA	A
    STA	SUPSPC	;SUPPRESS LEADING ZEROS
    JMP	PRT1	;PRINT SPACE REMAINING
;
;(fall into exit)
;Exit -	all done, restore stack
EXIT:	LHLD	STACK	;GET OLD STACK
    SPHL		;MOVE TO STACK
    RET		;..AND RETURN
;
;Temporary storage area
;
I:	DW	0
J:	DW	0
JG:	DW	0
GAP:	DW	0
;
BLKSHF:	DB	0	;# SHIFTS TO MULT BY SEC/BLK
BLKMSK:	DB	0	;SEC/BLK - 1
BLKMAX:	DW	0	;HIGHEST BLOCK # ON DRIVE
DIRMAX:	DW	0	;HIGHEST FILE #	IN DIRECTORY
TOTSIZ:	DW	0	;TOTAL SIZE OF ALL FILES
TOTFIL:	DW	0	;TOTAL NUMBER OF FILES
LINCNT:	DB	0	;COUNT OF LINES	ON SCREEN
TBLOC:	DW	0	;POINTER TO START OF NAME TABLE
NEXTT:	DW	0	;NEXT TABLE ENTRY
COUNT:	DW	0	;ENTRY COUNT
SCOUNT:	DW	0	;# TO SORT
SWITCH:	DB	0	;SWAP SWITCH FOR SORT
SUPSPC:	DB	0FFH	;LEADING SPACE FLAG FOR	DECIMAL	RTN.
BUFAD:	DW	BASE+80H ;OUTPUT ADDR
		DS	60	;STACK AREA
STACK:	DS	2	;SAVE OLD STACK	HERE
SOPFLG:	DS	1	;SET TO	'S' TO ALLOW SYS FILES TO PRINT
USERNO:	DS	1	;CONTAINS CURRENT USER NUMBER
TEMP:	DS	2	;SAVE DIR ENTRY
VERFLG:	DS	1	;VERSION FLAG
MPMFLG:	DS	1	;MP/M FLAG
LZFLG:	DS	1	;0 WHEN	PRINTING LEADING ZEROS
ORDER	EQU	$	;ORDER TABLE STARTS HERE
;
;BDOS equates
;
RDCHR	EQU	1	;READ CHAR FROM	CONSOLE
WRCHR	EQU	2	;WRITE CHR TO CONSOLE
PRINT	EQU	9	;PRINT CONSOLE BUFF
CONST	EQU	11	;CHECK CONS STAT
SELDSK	EQU	14	;SELECT	DISK
FOPEN	EQU	15	;0FFH=NOT FOUND
FCLOSE	EQU	16	;   "	"
FSRCHF	EQU	17	;   "	"
FSRCHN	EQU	18	;   "	"
CURDSK	EQU	25	;GET CURRENTLY LOGGED DISK NAME
GALLOC	EQU	27	;GET ADDRESS OF	ALLOCATION VECTOR
CURDPB	EQU	31	;GET CURRENT DISK PARAMETERS
CURUSR	EQU	32	;GET CURRENTLY LOGGED USER NUMBER (2.x ONLY)
;
END
