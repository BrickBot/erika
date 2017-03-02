#define PR (H'2')
#if (PR == 2)
	#define stampato
#endif
#define MAXADDR 0xb
#define SLICE (H'40' / (MAXADDR + 1))
#define TOK	  (PR * SLICE)
#if	(PR == 1)
	#define DebugCircuit
#endif
;per comunicare se bisogna gestire o meno l'ack di risposta (0 no, 1 si')
#define RCX	1
;per implementare assegnamento indirizzo via IR
;#if (PR == 3)
#define ADDRESSSELECTOR 1
;#else
;#define ADDRESSSELECTOR 0
;#endif

#define SENDRETRIES	10
#define TURNRETRIES 5
#define	IRRETRIES	10

		PROCESSOR	16F876;debug 
		INCLUDE		"P16F876.INC"
		RADIX		DEC
		ERRORLEVEL	-302

		;Setup of PIC configuration bits

		;HS Oscillator
		;Disable Watch Dog Timer
		;Enable power up timer
		;Disable code protect
		;Disable Low Voltage Programming

		__CONFIG _HS_OSC & _PWRTE_ON & _CP_OFF & _WDT_OFF & _LVP_OFF
#ifndef stampato
TXPIN	EQU		2
TXENA	EQU		1
RXENA	EQU		0
RXPIN	EQU		4
#else
RXENA	EQU		0
TXENA	EQU		1
TXPIN	EQU		2
RXPIN	EQU		3
#endif



; bit dello status byte (layer IR)
TXACK		EQU		0
TXMSG		EQU		1
TXTURN		EQU		2
TXBLOCK		EQU		3
IRRFFULL	EQU		4
RCVIRMSG	EQU		5
IRMUSTTX	EQU		6
IRISTX		EQU		7

;bit dello status byteIRRF 
IRRCVISMSG	EQU		0
TURN		EQU		1
IRACKNOTYET	EQU		2
IRACKTO		EQU		4

; bit dello status byte 2 (layer RF)
GOTTURN		EQU		0
GOTMSG		EQU		1
GOTACK		EQU		2
ORIGIN		EQU		3
RFIR1FULL	EQU		4
RFACKTO		EQU		5
TURNTO		EQU		6
RFMSGSENT	EQU		7
		

; bit dello status byte (Reservation Byte)
RESERVATION	EQU		0
RESTURN		EQU		1
RESMSG		EQU		2
RESBOTH		EQU		3
RESMSGSENT	EQU		4
RESACKRCV	EQU		5
ADDRESSRCVD	EQU		6
RCXBLOCKED	EQU		7


;;;;;;;;;;;Interrupts' MACROs;;;;;;;;;;;;;;;;;;
			
sti				macro
				banksel	INTCON
				bsf		INTCON,GIE
				endm

cli				macro
				banksel	INTCON
				bcf		INTCON,GIE
				endm	


IRTXEnable 		macro
				banksel	TXSTA
				bsf		TXSTA,TXEN
				bsf		PIE1,TXIE
				banksel	INTCON
				endm

IRTXDisable		macro
				banksel	TXSTA
				bcf		TXSTA,TXEN
				bcf		PIE1,TXIE
				banksel	PORTA
				endm



IRRXEnable		macro
				banksel	RCSTA
				bsf		RCSTA,CREN
				banksel	PIE1
				bsf		PIE1,RCIE
				banksel	PORTA
				endm

IRRXDisable		macro
				banksel	RCSTA
				bcf		RCSTA,CREN
				banksel	PIE1
				bcf		PIE1,RCIE
				banksel	PORTA
				endm


TMR1En			macro
				banksel	PIE1
				bsf		PIE1,TMR1IE
				banksel	T1CON
				bsf		T1CON,TMR1ON
				banksel	PORTA

				endm		

TMR1Dis			macro
				banksel	PIE1
				bcf		PIE1,TMR1IE
				banksel	T1CON
				bcf		T1CON,TMR1ON
				banksel	PORTA
				endm		


TMR2En			macro
				banksel	PIE1
				bsf		PIE1,TMR2IE
				banksel	T2CON
				bsf		T2CON,TMR2ON
				banksel	PORTA
				endm		

TMR2Dis			macro
				banksel	PIE1
				bcf		PIE1,TMR2IE
				banksel	T2CON
				bcf		T2CON,TMR2ON
				banksel	PORTA
				endm		

			
PEIEEn			macro
				banksel	INTCON
				bsf		INTCON,PEIE
				endm
			
PEIEDis			macro
				banksel	INTCON
				bcf		INTCON,PEIE
				endm

BeginIRTX		macro
				bsf		StatusByteIR,IRMUSTTX
				bcf		StatusByteIR,IRISTX
				endm

ContinueIRTX	macro
				bsf		StatusByteIR,IRMUSTTX
				bsf		StatusByteIR,IRISTX
				endm

EndIRTX			macro
				bcf		StatusByteIR,IRMUSTTX
				bcf		StatusByteIR,IRISTX
				bcf		StatusByteIR,TXTURN
				bcf		StatusByteIR,TXBLOCK
				bcf		StatusByteIR,TXACK
				bcf		StatusByteIR,TXMSG
				endm
;;;;;;;;;;;;Utility MACROs;;;;;;;;;;;;;;;;;;;;
		
;;;;;;;;;;;;;END MACROs;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;; PROGRAM VARIABLEs SPACE;;;;;;;;;;;;,
	CBLOCK		0X20
; le dimensioni del RFIRBuffer sono stabilite secondo la seguente
;considerazione: una volta ricevuto un messaggio RF, si procede a
;costruire un buffer che abbia ff fa Length Address [0-16 Byte] Check
RFIR1Buffer		;address 0x20
	ENDC
	CBLOCK		0x35
IRRFBuffer		;address 0x35
	ENDC
	CBLOCK		0x45
IRAckBuf		;address 0x45
	ENDC
	CBLOCK		0x48
Count					
	ENDC
	CBLOCK		0x4b
AckTurnBuffer
TurnRetries	
IRMsgRetries		
	ENDC
	CBLOCK		0x4e
VeriByte	
BitCounter		
Byte			
TxByte			
RxByte			
RefByte			
SByte			
Result			
RxByteNum		
IRLength		
RFLength		
MsgCode			
Parity			
ByteToInvert 	
InvertedByte 	
InvLoopByte		
InvLoopCount	
InvTemp			
ParLoopCount	
SIRLoop			
RIRLoop
Check			
MyCheck			
MyAddr
MaxAddr
TimeOutK;timeout constant passed by IR
IRAddress			
RFAddress			
TempStatus		
IRFSR			
RFFSR			
Complement		
ByteNum			
ByteToSend		
ByteCounter		
LengthLoop		
Sum				
AckWaited		
StatusByteIR		
StatusByteRF	
StatusByteRES
StatusByteIRRF
OkByte			
ToHaveTurn
LastSender
NextSender		
IRTOByte
LengthAppoggio
AddressAppoggio
SendTryNumber
	ENDC


;CBLOCK		0xA0
;StatusByteRES2
;ENDC

;RFIR2Buffer		RES		21
;;;;;;;;;;;;END VARIABLES;;;;;;;;;;;;;;;;;;;;;;;




; RESET VECTOR
; Start Point at CPU Reset		
		ORG		0x00
 		goto	Start

		ORG		0x04
;		testo se l'interruzione proviene da seriale in invio

		bsf		PORTB,6
		movfw	STATUS
		movwf	TempStatus
		btfsc	PIR1,TMR2IF
		goto	IRACKTimer
		btfsc	PIR1,TMR1IF
		goto	RFACKTURNTimer
		btfsc	PIR1,TXIF
		goto	IRSend
		btfsc	PIR1,RCIF
		goto	IRReceive
		movlw	B'00110011'
		andwf	PIR1,1
		bsf		STATUS,RP0
		btfsc	PIE1,7
		bcf		PIE1,7
		btfsc	PIE1,6
		bcf		PIE1,6
		btfsc	PIE1,3
		bcf		PIE1,3
		btfsc	PIE1,2
		bcf		PIE1,2
		bcf		STATUS,RP0
		goto	HandlerEnd

;*********Timer Interrupt handlers*******
IRACKTimer
		decfsz	IRTOByte,1
		goto	IRACKTimerGoOn
		TMR2Dis
		bcf		PIR1,TMR2IF
		bsf		StatusByteIRRF,IRACKTO
		decfsz	IRMsgRetries,1
		goto	IRACKTimerGoOn

		bcf		StatusByteIRRF,IRACKTO
#if (RCX == 1)
		bcf		StatusByteRF,RFIR1FULL
#endif
		clrf	ByteNum
		bcf		StatusByteRES,RESACKRCV
		bcf		StatusByteIR,RCVIRMSG
		bcf		PIR1,TMR2IF
		goto	RESBOTHTest

IRACKTimerGoOn
		bcf		PIR1,TMR2IF
		goto	HandlerEnd		


RFACKTURNTimer	
		TMR1Dis
		btfss	StatusByteIRRF,TURN	
		goto	SubTurnTO
SubAckTO
		bsf		StatusByteRF,RFACKTO
		goto	SubGoOn
SubTurnTO				
		bsf		StatusByteRF,TURNTO

SubGoOn		
		bcf		PIR1,TMR1IF
		goto	HandlerEnd
;****************************************



;***********IR Receiving Routine********
IRReceive
		bsf		PORTB,3
		movfw	FSR
		movwf	RFFSR
		movfw	IRFSR
		movwf	FSR
Waiting55
		movfw	ByteNum
		movwf	SByte
		movlw	0
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Waitingff	
		movfw	RCREG
		movwf	SByte
;N.B		
		btfsc	StatusByteIR,IRRFFULL
		goto	Resetting	
		bsf		StatusByteIR,RCVIRMSG	

		movlw	H'55'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting
		incf	ByteNum,1
		goto	ReceiveEnd		

Waitingff
		movfw	ByteNum
		movwf	SByte
		movlw	1
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Waiting00


		movfw	RCREG
		movwf	SByte
		movlw	H'ff'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	TryNew55


		goto	GoFFON
TryNew55
		movlw	H'55'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting

		goto	ReceiveEnd
GoFFON
		incf	ByteNum,1
		goto	ReceiveEnd


Waiting00
		movfw	ByteNum
		movwf	SByte
		movlw	2
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingType	

		
		movfw	RCREG
		movwf	SByte
		movlw	H'00'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting


		incf	ByteNum,1
		goto	ReceiveEnd


WaitingType
		movfw	ByteNum
		movwf	SByte
		movlw	3
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingNotType


		movfw	RCREG
		movwf	SByte
		movlw	H'f7'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	ProvaFA


		bsf		StatusByteIRRF,IRRCVISMSG
		movlw	H'F7'
		movwf	Check
		incf	ByteNum,1
		goto	ReceiveEnd
ProvaFA	movlw	H'fa'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	ProvaF3


		bsf		StatusByteIRRF,IRRCVISMSG
		movlw	H'fa'
		movwf	Check
		incf	ByteNum,1
		goto	ReceiveEnd
ProvaF3 movlw	H'f3'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting


		bcf		StatusByteIRRF,IRRCVISMSG
		movlw	H'F3'
		movwf	Check
		incf	ByteNum,1
		goto	ReceiveEnd


WaitingNotType
		movfw	ByteNum
		movwf	SByte
		movlw	4
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingLength
		movlw	h'35'			;indirizzamento indiretto: verra' utilizzato
		movwf	FSR
		movfw	RCREG
		movwf	SByte
		movlw	H'08'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Prova05
		movlw	1				;il caso di invio singolo viene trattato
		movwf	IRLength			;come invio multiplo di lunghezza 1
		movwf	RIRLoop
		movlw	3
		addwf	ByteNum,1
		goto	ReceiveEnd
Prova05	movlw	H'05'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Prova0C
		incf	ByteNum,1
		goto	ReceiveEnd
Prova0C	movlw	H'0c'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting
		movlw	5
		addwf	ByteNum,1
		goto	ReceiveEnd


WaitingLength
		movfw	ByteNum
		movwf	SByte
		movlw	5
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingNotLength
;		;leggo la parita' PRIMA di leggere il byte
		btfss	RCSTA,0
		goto	LParZero
LParUno
		movlw	01
		movwf	SByte
		goto	LParCompare
LParZero
		clrf	SByte
LParCompare
		movfw	RCREG
		movwf	Byte
		movwf	RxByte
		call	ParityEval
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting
;accertata la parita'
		movfw	RxByte
		movwf	IRLength
		movwf	RIRLoop
		addwf	Check,1
		incf	IRLength,1
		incf	ByteNum,1
		goto	ReceiveEnd
		

WaitingNotLength
		movfw	ByteNum
		movwf	SByte
		movlw	6
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingByte
		;leggo la parita' PRIMA di leggere il byte
		btfss	RCSTA,0
		goto	NLParZero
NLParUno
		movlw	01
		movwf	SByte
		goto	NLParCompare
NLParZero
		clrf	SByte
NLParCompare
		movfw	RCREG
		
		movwf	RxByte
		movwf	Byte
		call	ParityEval
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting
;accertata la parita'
		incf	ByteNum,1
		goto	ReceiveEnd
		

WaitingByte
		movfw	ByteNum
		movwf	SByte
		movlw	7
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingNotByte
		;leggo la parita' PRIMA di leggere il byte;
		btfss	RCSTA,0
		goto	BParZero
BParUno
		movlw	01
		movwf	SByte
		goto	BParCompare
BParZero
		clrf	SByte
BParCompare
		movfw	RCREG
		movwf	Byte
		movwf	RxByte
		call	ParityEval
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting
;;accertata la parita'
		movfw	RxByte
		movwf	INDF
		addwf	Check,1
		incf	ByteNum,1
		goto	ReceiveEnd


WaitingNotByte
		movfw	ByteNum
		movwf	SByte
		movlw	8
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingCheck
		movfw	RCREG
		decfsz	RIRLoop,1
		goto	Ripeti
		incf	ByteNum,1
		goto	ReceiveEnd
Ripeti	incf	FSR,1
		decf	ByteNum,1
		goto	ReceiveEnd


WaitingCheck
		movfw	ByteNum
		movwf	SByte
		movlw	9
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	WaitingNotCheck
		movfw	Check
		movwf	SByte
		clrf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	CheckNotZero
		incf	FSR,1		
		movfw	RCREG
		movwf	INDF
		movwf	SByte
		movlw	0x0f
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting
		goto	CheckEnd
CheckNotZero
		incf	FSR,1		
		movfw	RCREG
		movwf	INDF
		movfw	Check
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting
CheckEnd
		incf	ByteNum,1
		goto	ReceiveEnd


WaitingNotCheck
		movfw	RCREG
		movfw	ByteNum
		movwf	SByte
		movlw	10
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	Resetting

ShowWithLeds
		btfss	StatusByteIRRF,IRRCVISMSG
		goto	IRAckEnd

IRMsgEnd
;configurazione automatica: il pic manda l'indirizzo
#if (ADDRESSSELECTOR == 1)
		btfsc	StatusByteRES,ADDRESSRCVD
		goto	NormalMode
;il PRIMO messaggio ricevuto ha il seguente formato: 
;55 ff 00 fa 05 02 fd myaddr ~myaddr maxaddr ~maxaddr check ~check
AddressMode

		movlw	0x35
		movwf	FSR
		movfw	INDF
		movwf	MyAddr
		incf	FSR,1
		movfw	INDF
		movwf	MaxAddr
		incf	FSR,1
		movfw	INDF
		movwf	TimeOutK
		bsf		StatusByteRES,ADDRESSRCVD
		goto	Resetting
		
NormalMode
#endif
		bsf		PORTB,2
		bsf		StatusByteIRRF,IRACKNOTYET
;notifichiamo l'avvenuto riempimento del buffer		
		bsf		StatusByteIR,IRRFFULL
;prepariamo il numero massimo di tentativi di invio di questo messaggio
		movlw	SENDRETRIES
		movwf	SendTryNumber
		bcf		StatusByteRF,RFMSGSENT

;prepariamo le variabili per l'invio dell'ack
		BeginIRTX
		bsf		StatusByteIR,TXACK
		bcf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXTURN
		bcf		StatusByteIR,TXBLOCK
		goto	Resetting

IRAckEnd
;bisogna liberare il buffer da RF a IR pieno finora (aggiungere in un secondo momento)
#if (RCX == 1)
		bcf		StatusByteRF,RFIR1FULL
#endif


		bcf		StatusByteRES,RESACKRCV
		TMR2Dis
;il codice seguente perchè dopo aver inviato un messaggio IR al RCX ci aspettiamo l'ack
;in tal caso tutte le prenotazioni di invio da parte del pic al rcx devono essere
;posticipate al momento in cui l'ack è ricevuto, cioè questo.
		clrf	ByteNum
		bcf		StatusByteIR,RCVIRMSG
;		bcf		PIR1,RCIF
		movfw	FSR
		movwf	IRFSR
		movfw	RFFSR
		movwf	FSR
		goto	RESBOTHTest


Resetting
		bcf		StatusByteIR,RCVIRMSG
		bcf		StatusByteIRRF,IRRCVISMSG
		clrf	ByteNum
ReceiveEnd
		goto	IRHandlersEnd


;***********IR SENDING INTERRUPT ROUTINE*******************
IRSend

		movfw	SIRLoop
		movwf	SByte
		clrf	RefByte
		call	Compare
		movwf	Result
		btfsc	Result,0
		goto	IRMsgSendEnd
		movfw	FSR
		movwf	RFFSR
		movfw	IRFSR
		movwf	FSR
		btfss	Complement,0
		goto	IRNormalByte
		goto	IRComplByte
IRNormalByte
		bsf		Complement,0
		movfw	INDF
		goto	IRGo
IRComplByte
		bcf		Complement,0
		decf	SIRLoop,1
		comf	INDF,0
		incf	FSR,1
IRGo
		movwf	Byte
		movwf	TxByte
		call	ParityEval	;
		btfsc	Parity,0	;
		goto	ParOne		;
ParZero						;
		bsf		STATUS,RP0	;
		bcf		TXSTA,TX9D	;
		goto	GoOn		;
ParOne						;				
		bsf		STATUS,RP0	;
		bsf		TXSTA,TX9D	;
GoOn						;
		bcf		STATUS,RP0	;
		movfw 	TxByte
#if		(PR == 1)
;		movwf	PORTB
#endif
		movwf	TXREG		;
		goto	IRHandlersEnd

IRMsgSendEnd
		banksel	TXSTA
poll	btfss	TXSTA,TRMT
		goto	poll
		bcf		TXSTA,TXEN
		banksel	PORTA
		PEIEDis


;***********TXACKTEST*********************
TXACKTest
		btfss	StatusByteIR,TXACK
		goto	TXBLOCKTest
		

;IR ACK SENT, dobbiamo bloccare RCX
#if		(ADDRESSSELECTOR == 1)
;il primo ack trasmesso sarà quello al messaggio di configurazione
		btfsc	StatusByteRES,ADDRESSRCVD
		goto	NormalAckTx

AddressAckTx
		bsf		StatusByteRES,ADDRESSRCVD
		goto	EndIRTransmission

NormalAckTx
#endif
		bcf		StatusByteIRRF,IRACKNOTYET
;se si è già ricevuto l'RF Ack non ha senso bloccare l'RCX
		btfss	StatusByteIR,IRRFFULL
		goto	RESBOTHTest		

		BeginIRTX
		bsf		StatusByteIR,TXBLOCK
		bcf		StatusByteIR,TXTURN
		bcf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXACK
		goto	IRSENDENDHandlerEnd


;*****************TXBLOCKTEST***********************
TXBLOCKTest
		btfss	StatusByteIR,TXBLOCK
		goto	TXMSGTest
		

;IR BLOCK SENT
		goto	RESBOTHTest


;*****************TXMSGTEST*************************
TXMSGTest
		btfss	StatusByteIR,TXMSG												;
		goto	TXTURNTest
		


;IR Message SENT
;ATTN: codice temporaneo per permettere anche in assenza di ack di proseguire;;;;
#if (RCX == 0)
		bcf		StatusByteRF,RFIR1FULL
#else
		bsf		StatusByteRES,RESACKRCV
#endif
;setup del timeout di ritrasmissione IR-->65535 * 7 / 5 circa 100000
		movlw	7
		movwf	IRTOByte
		TMR2En
		bcf		StatusByteRES,RESMSG
		bsf		StatusByteRES,RESMSGSENT
		goto	EndIRTransmission

;*************TXTURNTEST*****************************
TXTURNTest
		btfss	StatusByteIR,TXTURN
		goto	EndIRTransmission
		
		bcf		StatusByteRES,RESTURN
		bcf		StatusByteRES,RESBOTH	
		bcf		StatusByteRES,RCXBLOCKED
		goto	RESMSGTest


;************RESBOTHTEST*******************************
RESBOTHTest
		btfss	StatusByteRES,RESBOTH
		goto	RESMSGTest
;entrambi prenotati, può essere che msg sia già stato inviato******************
		btfss	StatusByteRES,RESMSGSENT
		goto	MSGToSend
		BeginIRTX
		bcf		StatusByteIR,TXBLOCK
		bsf		StatusByteIR,TXTURN
		bcf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXACK
		bcf		StatusByteRES,RESMSGSENT
		goto	IRSENDENDHandlerEnd
MSGToSend
		BeginIRTX
		bcf		StatusByteIR,TXBLOCK
		bcf		StatusByteIR,TXTURN
		bsf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXACK
		goto	IRSENDENDHandlerEnd


;****************RESMSGTEST***********************
RESMSGTest
		btfss	StatusByteRES,RESMSG
		goto	RESTURNTest
		BeginIRTX
		bcf		StatusByteIR,TXBLOCK
		bcf		StatusByteIR,TXTURN
		bsf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXACK
		goto	IRSENDENDHandlerEnd

;***************RESTURNTEST***********************
RESTURNTest
		btfss	StatusByteRES,RESTURN
		goto	EndIRTransmission	
		BeginIRTX
		bcf		StatusByteIR,TXBLOCK
		bsf		StatusByteIR,TXTURN
		bcf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXACK
		goto	IRSENDENDHandlerEnd



EndIRTransmission
		EndIRTX
		IRTXDisable
		IRRXEnable
		PEIEEn		
		goto	IRSENDENDHandlerEnd


IRHandlersEnd
;		bcf		PIR1,RCIF
		movfw	FSR
		movwf	IRFSR
		movfw	RFFSR
		movwf	FSR
		

IRSENDENDHandlerEnd

HandlerEnd
		bcf		PORTB,6
		bcf		PORTB,3
		bcf		PORTB,2
		movfw	TempStatus
		movwf	STATUS
		retfie


Start
		bsf		STATUS,RP0

		movlw	B'00000111'
		movwf	ADCON1

#ifndef stampato
		movlw	B'11111001'
		movwf	TRISA
	
		movlw	B'00000000'
		movwf	TRISB

		movlw	B'11111110'
		movwf	TRISC
#else
		movlw	B'11111111'
		movwf	TRISA
		
		movlw	B'01111111'
		movwf	TRISB

		movlw	B'11111000'
		movwf	TRISC
#endif						

;		Transmissione a 9 bit
;		Trasmissione abilitata
;		Asynchronous mode
;		Low Speed
		movlw	B'11100010'	
		movwf	TXSTA

;		2400 bps
		movlw 	129
		movwf	SPBRG

;		0xff in PR2 per l'ack ir timeout
		movlw	0xff
		movwf	PR2

;		fine accesso al bank 1, ora bank 0
		bcf		STATUS,RP0

;		Serial port enabled
;		9 Bit Receive Enabled
;		Continuous Receive
;		Address detection disabled
		movlw	B'11110000'
		movwf	RCSTA

; 		TMR0 Module Prepared
;		Internal Clock(Fosc / 4)
;		Oscillator Disabled
;		Prescaler 1:1
		movlw	B'11000100'
		movwf	T1CON

; 		TMR2 Module Prepared
;		Prescaler 1:16
;		Postscaler 1:16
; in tal modo è possibile avere un incremento ogni 40 cicli istruzioni
; cioè ogni 8 us.
		movlw	B'11111011'
		movwf	T2CON

;disabilitiamo Invio
		call	TxDisable
;abilitiamo Ricezione
		call	RxEnable

		clrf	PORTB
		clrf	ByteNum
		clrf	StatusByteIR
		clrf	StatusByteRF
		clrf	StatusByteRES
		clrf	StatusByteIRRF
		bsf		PORTB,7	
	
#if (ADDRESSSELECTOR == 1)
;*****************ROUTINE RILEVAMENTO INDIRIZZO***************************
		cli
		IRTXDisable
		IRRXEnable
		PEIEEn
		sti

WaitForAddress
		btfss	StatusByteRES,ADDRESSRCVD
		goto	WaitForAddress
		bcf		StatusByteRES,ADDRESSRCVD
		BeginIRTX
		bsf		StatusByteIR,TXACK
		bcf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXTURN
		bcf		StatusByteIR,TXBLOCK
		call	IRAckSend

IIWaitForAddress		
		btfss	StatusByteRES,ADDRESSRCVD
		goto	IIWaitForAddress
		

;*************************************************************************
#else
		movlw	PR
		movwf	MyAddr
		movlw	MAXADDR
		movwf	MaxAddr
		movlw	TOK
		movwf	TimeOutK
#endif

;chi ha l'indirizzo ha il turno, per mezzo di ORIGIN	
		movfw	MyAddr
		movwf	SByte
		movlw	0
		movwf	RefByte
		call	Compare
		movwf	Result
		btfsc	Result,0
		bsf		StatusByteRF,ORIGIN

; accendiamo il led per testare il power on
		call	ViewDelay
		bcf		PORTB,7
		
;;;;;;;;;;;;;;;;;;;MAINLOOP;;;;;;;;;;;;;;;;;;;
MainLoop
		sti
		cli

provaled5
		btfss	PORTB,5
		goto	SetLed5
ResetLed5
		bcf		PORTB,5
		goto	EndLed5
SetLed5	bsf		PORTB,5
EndLed5

;*************TIMEOUTS' SECTION************************
TURNTOVerify
		btfss	StatusByteRF,TURNTO
		goto	RFACKTOVerify
;Si è verificato un TurnTimeOut

		call	GiveTurn
		goto	MainLoop
;Non v'è stato TurnTimeOut verifichiamo il RFACKTimeOut
RFACKTOVerify
		btfss	StatusByteRF,RFACKTO
		goto	IRACKTOVerify
;Si è verificato un RFACKTimeOut
		decfsz	SendTryNumber,1
		goto	RepeatSend

		bcf		StatusByteRF,RFMSGSENT
		bcf		StatusByteRF,RFACKTO
		goto	AckStuff
RepeatSend

		bcf		StatusByteRF,RFMSGSENT
		bcf		StatusByteRF,RFACKTO
		call	GiveTurn
		goto	MainLoop
;Non v'è stato RFACKTimeOut verifichiamo il IRACKTOVerify
IRACKTOVerify
		btfss	StatusByteIRRF,IRACKTO
		goto	IRSection

		bcf		StatusByteIRRF,IRACKTO
		goto	MsgStuff
;************END TIMEOUTS' SECTION*********************


;*******************IR SECTION*************************
IRSection
		cli
;Verifichiamo innanzitutto che si sia o meno in ricezione

		btfsc	StatusByteIR,RCVIRMSG
		goto	IRRXSetup
;Verifichiamo che vi sia o meno da trasmettere qualcosa
		btfss	StatusByteIR,IRMUSTTX
		goto	IRRXSetup

;siamo in attesa di ACK a un messaggio IR***
		btfss	StatusByteRES,RESACKRCV
		goto	IRTXSetup

				
;******Abilitiamo la sezione ricevente*****************
IRRXSetup

		IRTXDisable
		IRRXEnable
		PEIEEn
		sti
		goto	RFSection

;oppure******abilitiamo la sezione trasmittente********
IRTXSetup
;******si verifica se qualcosa già in trasmissione***		
		btfsc	StatusByteIR,IRISTX
		goto	RFSection
;********c'è qualcosa da trasmettere, verifichiamo cosa
;******** e invochiamo la routine+++++++++++++++++*****
		btfsc	StatusByteIR,TXACK
		call	IRAckSend
		btfsc	StatusByteIR,TXMSG
		call	IRMsgSend
		btfsc	StatusByteIR,TXBLOCK
		call	IRBlockSend
		btfsc	StatusByteIR,TXTURN
		call	IRTurnSend
		goto	RFSection
;*************END IR SECTION***************************


;*******************RF SECTION*************************
RFSection
		cli

		call	BeginDetect
;*************testiamo il tipo di pacchetto ricevuto***
		btfsc	StatusByteRF,GOTTURN
		goto	TurnManage
		btfsc	StatusByteRF,GOTMSG
		goto	MsgManage
		btfsc	StatusByteRF,GOTACK
		goto	AckManage
;*******in questo caso o c'è stato un errore nella*****
;ricezione o il messaggio non era a noi destinato******
		goto	MainLoop

;*************GOT TOKEN********************************
TurnManage
		sti
		cli

		bcf		StatusByteRF,GOTTURN		
		bsf		StatusByteIRRF,TURN
;***verifichiamo se abbiamo da trasmettere*************
		btfss	StatusByteIR,IRRFFULL
		goto	NothingToTx

SomethingToTx

;***si trasmette il contenuto del buffer***************
		btfsc	StatusByteRF,RFMSGSENT
		goto	MainLoop
		bsf		StatusByteRF,RFMSGSENT
		call	RFMsgSend


;++++++++++++++++++++++DEBUG++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;		movlw	25
;		movwf	Count
;		call	RDelayLoop
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		goto	MainLoop
NothingToTx

		call	GiveTurn
		goto	MainLoop

				
;**********END GOT TURN********************************


;************GOT MESSAGE*******************************
MsgManage
		sti
		cli

		bcf		StatusByteRF,GOTMSG
		call	RFAckSend

		;DEBUG TRY
		movlw	IRRETRIES
		movwf	IRMsgRetries
		
MsgStuff
		cli
		bcf		StatusByteRES,RESACKRCV

		btfsc	StatusByteIR,RCVIRMSG
		goto	IRRcvORNoIRTurn
		
		btfsc	StatusByteIRRF,IRACKNOTYET
		goto	IRRcvORNoIRTurn
		
NoIRRcv
		btfsc	StatusByteIR,IRMUSTTX
		goto	MSGIRTx
MSGNoIRTx

		BeginIRTX
		bsf		StatusByteIR,TXMSG
		bcf		StatusByteIR,TXTURN
		bcf		StatusByteIR,TXBLOCK
		bcf		StatusByteIR,TXACK
		goto	MainLoop
MSGIRTx
		btfsc	StatusByteIR,TXTURN
		goto	MSGReservation
IRRcvORNoIRTurn
		btfss	StatusByteRES,RESTURN
		goto	MSGReservation
MSGRESBothTurn
		bsf		StatusByteRES,RESBOTH
MSGReservation
		bcf		StatusByteRES,RESMSGSENT
		bsf		StatusByteRES,RESMSG

		goto	MainLoop		
;*********END GOT MESSAGE******************************


;************GOT ACK***********************************
AckManage
		sti
		cli


		bcf		StatusByteRF,GOTACK

AckStuff
		bcf		StatusByteIR,IRRFFULL
		call	GiveTurn
		cli
		
		btfss	StatusByteRES,RCXBLOCKED
		goto	MainLoop		

		btfsc	StatusByteIR,RCVIRMSG
		goto	ACKIRTx
	
		btfsc	StatusByteIRRF,IRACKNOTYET
		goto	ACKIRTx

		btfsc	StatusByteIR,IRMUSTTX
		goto	ACKIRTx

		BeginIRTX
		bcf		StatusByteIR,TXMSG
		bsf		StatusByteIR,TXTURN
		bcf		StatusByteIR,TXACK
		bcf		StatusByteIR,TXBLOCK
		goto	MainLoop

ACKIRTx
		btfsc	StatusByteIR,TXMSG
		goto	TURNReservation
		btfss	StatusByteRES,RESMSG
		goto	TURNReservation
		bsf		StatusByteRES,RESBOTH
TURNReservation
		bsf		StatusByteRES,RESTURN

		goto	MainLoop		
;*********END GOT ACK**********************************
	




;;;;Routine to detect begin of RF transmission;;;
BeginDetect
provaled4
		btfss	PORTB,4
		goto	SetLed4
ResetLed4
		bcf		PORTB,4
		goto	EndLed4
SetLed4	bsf		PORTB,4
EndLed4
		sti
		call	TxDisable
		call	RxEnable
		cli
;prova	
		movlw	90
		movwf	VeriByte
FirstZero
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		
		goto	FailTx
		decfsz	VeriByte,1
		goto	FirstZero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;Following instructions let the short impulse end;;;
FirstRemainingZero
#ifndef	stampato
		btfss	PORTA,RXPIN
#else
		btfss	PORTC,RXPIN
#endif		

		goto	FirstRemainingZero

FirstShortImpulse
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		

		goto	FirstShortImpulse

		movlw	130
		movwf	Count
		call	RDelayLoop

		movlw	30
		movwf	VeriByte
	
SecondZero
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		
		goto	FailTx
SecondContinue
		decfsz	VeriByte,1
		goto	SecondZero
SecondRemainingZero
#ifndef	stampato
		btfss	PORTA,RXPIN
#else
		btfss	PORTC,RXPIN
#endif		
		goto	SecondRemainingZero
SecondShortImpulse
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		

		goto	SecondShortImpulse
;;;FSR is a pointer to the first location of Buffer
;it starts from 24 because it's necessary to send IR header first

		movlw	0x4b
		movwf	FSR
		movlw	3
		movwf	Count
		call	RDelayLoop		
		movlw	8
		movwf	BitCounter
		nop
		bcf		STATUS,C
		clrf	OkByte
		bsf		Parity,0
;cycle to receive the eight bits
ToRcv55
		rlf		OkByte,1
		movlw	13
		call	BitReceive
		iorwf	OkByte,1
		xorwf	Parity,1
		decfsz	BitCounter,1	
		goto	ToRcv55
;now we are ready to receive byte parity

		movfw	Parity
		andlw	0x01
		movwf	RefByte
		movlw	13
		call	BitReceive
		movwf	SByte
		call	Compare
		movwf	Result

		btfss	Result,0
		goto	FailTx
;at this point INDF contains the received  byte in an inverted form
		movfw	OkByte
		movwf	ByteToInvert
		call	InvertByte
		movwf	OkByte

		movwf	SByte
		movlw	0x55
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	FailTx
	
;got	0x55
		clrf	VeriByte


		
;let the stop bit end: it will be followed by  byteNum's start bit
StopBit55	
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		
		goto	StopBit55


;now there's byte number reception:
;let's prepare the reading		
		movlw	3
		movwf	Count
		call	RDelayLoop		
		movlw	8
		movwf	BitCounter
		nop
		bcf		STATUS,C
		clrf	LengthAppoggio
		clrf	Parity
		bsf		Parity,0
;cycle to receive the eight bits
NumberRecv
		rlf		LengthAppoggio,1
		movlw	13
		call	BitReceive
		iorwf	LengthAppoggio,1
		xorwf	Parity,1
		decfsz	BitCounter,1	
		goto	NumberRecv
;now we are ready to receive byte parity
		movfw	Parity
		andlw	0x01
		movwf	RefByte
		movlw	11
;		movlw	8
		call	BitReceive
		movwf	SByte
		call	Compare
		movwf	Result
		incf	VeriByte,1		
		btfss	Result,0	
		goto	FailTx

;at this point INDF contains the received  byte in an inverted form
NumberNoProblem
		movfw	LengthAppoggio
		movwf	ByteToInvert
		call	InvertByte
		movwf	LengthAppoggio
		movwf	MsgCode

;a new transmission is correctly begun; let's disable Turn Timeout
		btfsc	StatusByteIRRF,TURN			
		goto	AfterTMR1
		TMR1Dis	
		bcf		PORTB,7
AfterTMR1
;msg has the following format [MsgCode(3bit),RFLength(5bit)]
;ANDing RFLength with 00011111 we leave only length in the byte
		movlw	0x1f
		andwf	LengthAppoggio,1
;ANDing MsgCode with 11100000 we leave only code in the byte
		movlw	0x60
		andwf	MsgCode,1
		bcf		STATUS,C
;Rotating it we have a byte of the form 0CCC0000
		rrf		MsgCode,1
;Finally swapping it the form is 00000CCC
		swapf	MsgCode,1
;We have to test if a IR message is to be send to the RCX yet		
		incf	MsgCode,1
		decfsz	MsgCode,1
		goto	NoMsg
Msg
		btfsc	StatusByteRF,RFIR1FULL
		goto	RFBufferFull
		movlw	0x24
		movwf	FSR
NoMsg
;we have to decrement RFLength since we'll receive Address byte separately from the others
		decf	LengthAppoggio,1
;i turni li m
		
RFRXGoOn		
;let the stop bit begin	(no matter if parity was 1)
#ifndef	stampato
		btfss	PORTA,RXPIN
#else
		btfss	PORTC,RXPIN
#endif		
		goto	FailTx


;let the stop bit end: it will be followed by address byte's start bit
NEndLoop	
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		
		goto	NEndLoop

		movlw	3
		movwf	Count
		call	RDelayLoop		
		movlw	8
		movwf	BitCounter
		nop
		bcf		STATUS,C
		clrf	AddressAppoggio
		bsf		Parity,0
;cycle to receive the eight bits
AddressRecv
		rlf		AddressAppoggio,1
		movlw	13
		call	BitReceive
		iorwf	AddressAppoggio,1
		xorwf	Parity,1
		decfsz	BitCounter,1	
		goto	AddressRecv
;now we are ready to receive byte parity

		movfw	Parity
		andlw	0x01
		movwf	RefByte
		movlw	8
		call	BitReceive
		movwf	SByte
		call	Compare
		incf	VeriByte,1		
		movwf	Result
		btfss	Result,0	
		goto	FailTx

AddressNoProblem
;at this point INDF contains the received  byte in an inverted form
		movfw	AddressAppoggio
		movwf	ByteToInvert
		call	InvertByte
		movwf	AddressAppoggio


	;now Address contains the address which message is to be delivered to
;here, we have to test if we have the same address
		movfw	MyAddr
		movwf	SByte
		movfw	AddressAppoggio
		andlw	H'0F'
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	FailTx
	
		btfss	StatusByteRES,RESERVATION
		goto	NoReservation
		swapf	AddressAppoggio,0
		andlw	0x0f
		movwf	SByte
		movfw	NextSender
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	FailTx
		bcf		StatusByteRES,RESERVATION

NoReservation		
;we have to test stop bit presence

#ifndef	stampato
		btfss	PORTA,RXPIN
#else
		btfss	PORTC,RXPIN
#endif		
		goto	FailTx
;1)	Let the cycle begin
ReceivingCycle
		movfw	LengthAppoggio
		movwf	RxByteNum
EndLoop	
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		
		goto	EndLoop
		movlw	5
		movwf	Count
		call	RDelayLoop		
		movlw	8
		movwf	BitCounter
		bcf		STATUS,C
		clrf	INDF
		clrf	Parity
		bsf		Parity,0
ByteRecv
		rlf		INDF,1
		movlw	13
		call	BitReceive
		iorwf	INDF,1
		xorwf	Parity,1
		decfsz	BitCounter,1	
		goto	ByteRecv
ParityReceive
		movfw	Parity
		andlw	0x01
		movwf	RefByte
		movlw	13
		call	BitReceive
		movwf	SByte
		call	Compare
		movwf	Result
		incf	VeriByte,1		
		btfss	Result,0	
		goto	FailTx
;		nop

;at this point INDF contains the received  byte in an inverted form
		movfw	INDF
		movwf	ByteToInvert
		call	InvertByte
		movwf	INDF

;now INDF contains byte in the right order
		incf	FSR,1
ParityOk
#ifndef	stampato
		btfss	PORTA,RXPIN
#else
		btfss	PORTC,RXPIN
#endif		
		goto	FailTx
;		nop

;now we check if the received byte was the last one

		decfsz	RxByteNum,1
		goto	EndLoop

EndOfTx
		decf	FSR,1
		movfw	INDF
		movwf	Check
;ItsAddr contains a byte of the form [Me, Sender]
		swapf	AddressAppoggio,0
		andlw	0x0f
		movwf	LastSender
		
CheckMsgType
		movfw	MsgCode
		movwf	SByte
		clrf	RefByte
		call	Compare
		movwf	Result
		btfsc	Result,0
		goto	MsgGoOn
TryAck	movlw	1
		movwf	RefByte
		call	Compare
		movwf	Result
		btfsc	Result,0
		goto	AckGoOn
TryTurn	movlw	2
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	FailTx
		goto	TurnGoOn
;caso in cui alla sessione precedente il messaggio non aveva ricevuto conferma e rimane lo stesso

MsgGoOn
#ifdef DebugCircuit
#endif


		movfw	AddressAppoggio
		movwf	IRAddress
		movfw	LengthAppoggio
		movwf	RFLength
		call	CheckSum
		movwf	Result
		btfss	Result,0
		goto	FailTx

		bsf		StatusByteRF,RFIR1FULL
		bsf		StatusByteRF,GOTMSG
		bcf		StatusByteRF,GOTACK
		bcf		StatusByteRF,GOTTURN

		sti
		return


AckGoOn

		
		btfss	AckWaited,7
		goto	FailTx
		movfw	AckWaited
		andlw	0x0F
		movwf	SByte
		movfw	LastSender
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	FailTx
		clrf	AckWaited
		TMR1Dis
		bcf		StatusByteRF,RFACKTO		
		bsf		StatusByteRF,GOTACK
		bcf		StatusByteRF,GOTTURN
		bcf		StatusByteRF,GOTMSG
		sti
		return


TurnGoOn

		bsf		PORTB,7
		bsf		StatusByteRF,GOTTURN
		bcf		StatusByteRF,GOTMSG
		bcf		StatusByteRF,GOTACK

		sti
		return		

FailTx

		btfsc	StatusByteRF,ORIGIN
		goto	FirstTimeTurn
		bcf		StatusByteRF,GOTMSG
		bcf		StatusByteRF,GOTACK
		bcf		StatusByteRF,GOTTURN
		sti
		return

FirstTimeTurn
		bcf		StatusByteRF,ORIGIN
		bcf		StatusByteRF,GOTMSG
		bcf		StatusByteRF,GOTACK
		bsf		StatusByteRF,GOTTURN
		return


RFBufferFull


BufferFullLoop
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		
		goto	BufferFullLoop
		movlw	3
		movwf	Count
		call	RDelayLoop		
		movlw	8
		movwf	BitCounter
		nop
		bcf		STATUS,C
		clrf	AddressAppoggio
		bsf		Parity,0
;cycle to receive the eight bits
RecvSenderAddr
		rlf		AddressAppoggio,1
		movlw	13
		call	BitReceive
		iorwf	AddressAppoggio,1
		xorwf	Parity,1
		decfsz	BitCounter,1	
		goto	RecvSenderAddr
;now we are ready to receive byte parity

		movfw	Parity
		andlw	0x01
		movwf	RefByte
		movlw	8
		call	BitReceive
		movwf	SByte
		call	Compare
		incf	VeriByte,1		
		movwf	Result
		btfss	Result,0	
		goto	FailTx

;at this point INDF contains the received  byte in an inverted form
		movfw	AddressAppoggio
		movwf	ByteToInvert
		call	InvertByte
		movwf	AddressAppoggio
		;now Address contains the address which message is to be delivered to
;here, we have to test if we have the same address
		
		movfw	MyAddr
		movwf	SByte
		movfw	AddressAppoggio
		movwf	RefByte
		movlw	H'0F'
		andwf	RefByte,1
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	FailTx

		swapf	AddressAppoggio,0
		andlw	0x0f
		movwf	SByte
		movfw	LastSender
		movwf	RefByte
		call	Compare
		movwf	Result
		btfsc	Result,0
		goto	FailTx
;Steps to allow the sender to reserve a sending possibility
		movfw	SByte
		movwf	NextSender
		bsf		StatusByteRES,RESERVATION
		goto	FailTx

CheckSum


		movlw	0x24
		movwf	FSR
		clrf	MyCheck
		clrf	SByte
		call	MCInit
		movfw	MyCheck
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	CheckNZ
CheckZ
		movlw	0x0f
		movwf	SByte
		movfw	Check
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		retlw	0
		retlw	1
CheckNZ
		movfw	Check
		movwf	SByte
		movfw	MyCheck
		movwf	RefByte
		call	Compare
		movwf	Result
;		btfss	Result,0
;		retlw	0
;debugging purpouse
		btfss	Result,0
		retlw	0		
		retlw	1
		
MCInit
		movlw	0xfa
		movwf	MyCheck
		movfw	IRAddress
		addwf	MyCheck,1
		movfw	RFLength
		addwf	MyCheck,1
; RFLength va decrementata perchè al check abbiamo già sommato l'indirizzo
		decf	RFLength,0
		movwf	RxByteNum
CLoop
		movfw	INDF
		addwf	MyCheck,1
		incf	FSR,1
		decfsz	RxByteNum,1
		goto	CLoop
		return

;Routine di buffer viewing;;;;;;;
Visualizza		
;		clrf	PORTB
;		bsf		PORTB,7
;		call	ViewDelay
;		call	ViewDelay
;		call	ViewDelay
;		call	ViewDelay
;		call	ViewDelay
;		bcf		PORTB,7
;		call	ViewDelay
;		call	ViewDelay
		movlw	0x24			;
		movwf	FSR				;
		movfw	RFLength			;
		movwf	RxByteNum		;
ViewLoop						;
		movfw	INDF
		movwf	PORTB
		call	ViewDelay
		incf	FSR,1			;
		decfsz	RxByteNum,1		;
		goto	ViewLoop
   		return					;	

ViewDelay						;	
		clrf	AckTurnBuffer			;	
		clrf	AckTurnBuffer+1		;
		movlw	30
		movwf	AckTurnBuffer+2		;
ViewDelayLoop					;
		nop						;
		nop						;
		decfsz	AckTurnBuffer,1		;
		goto	ViewDelayLoop	;
								;
		decfsz	AckTurnBuffer+1,1		;
		goto	ViewDelayLoop	;
								;
		decfsz	AckTurnBuffer+2,1		;
		goto	ViewDelayLoop	;
								;
		return					;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;IR Sending Routine;;;;;;
IRMsgSend
		cli


		ContinueIRTX
		
		movlw	0x20
		movwf	FSR

		movlw	0xff
		movwf	INDF
		incf	FSR,1

		movlw	0xfa
		movwf	INDF
		incf	FSR,1

		movfw	RFLength
		movwf	INDF
		incf	FSR,1

		movfw	IRAddress
		movwf	INDF	

	
		movlw	4
		movwf	SIRLoop
		movfw	RFLength
		addwf	SIRLoop,1
		movlw	0x20
		movwf	IRFSR
		clrf	Complement
		banksel	TXSTA
		bsf		TXSTA,TX9D
		banksel	PORTA
		movlw	0x55
		movwf	TXREG

		IRRXDisable
		IRTXEnable
		PEIEEn
		sti
		return		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;Bit Receiving Routine;;;;;;;;;											;   
BitReceive						;
		movwf	Count			;
		call	RDelayLoop		;
#ifndef	stampato
		btfsc	PORTA,RXPIN
#else
		btfsc	PORTC,RXPIN
#endif		

		goto	PinOne
PinZero							;
       	retlw	0				;
PinOne							;
		retlw	1				;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;Routine to compare 2 bytes;;
Compare							;
		movfw	SByte			;
		subwf	RefByte,1		;
		btfss	STATUS,Z		;
		goto	NEqual			;
Equal	retlw	1				;
NEqual	retlw	0				;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;Routine to invert a byte;;;;;;
InvertByte						;
		movlw	7				;
		movwf	InvLoopByte		;
		movfw	ByteToInvert	;
		movwf	InvTemp			;
InvLoop							;
		btfss	InvTemp,0		;
		goto	BitClear		;
BitSet							;
		bsf		InvertedByte,0	;
		goto	BitEnd			;
BitClear						;
		bcf		InvertedByte,0	;
		goto	BitEnd			;
BitEnd							;
		rlf		InvertedByte,1	;
		rrf		InvTemp,1		;
		decfsz	InvLoopByte,1	;
		goto	InvLoop			;
; last time InvertedByte has not;
;to be rotated, so we have to	;
;threat it as a case apart		;
		btfss	InvTemp,0		;
		goto	LastBitClear	;
LastBitSet						;
		bsf		InvertedByte,0	;
		goto	LastBitEnd		;
LastBitClear					;
		bcf		InvertedByte,0	;
		goto	LastBitEnd		;
LastBitEnd						;
		movfw	InvertedByte	;
		return					;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


RFMsgSend
		sti

		call	SendRoutine
		return

SendRoutine
		movlw 	0x35
		movwf	FSR
		movfw	IRLength
		movwf	LengthLoop

;address byte must be build according to the following format [myAddress, DestAddress]
;INDF contains Destination Address, we swap it, clear the 4 less significant bits, add our 4 bit address
;and swap again to have the byte. Then we send it and decrement LengthLoop as we have sent one more byte
		movfw	INDF
		movwf	RFAddress
;	In AckWaited va un byte della forma [AW,x,x,x,AckSenderAddr]
		movwf	AckWaited
		bsf		AckWaited,7
		
		incf	FSR,1 
		decf	LengthLoop,1

		;Preamble
		call	RxDisable
		call	TxEnable
		cli
		call	Intro
		;Start sending
		movlw	0x55
		movwf	Byte
		call	ByteStart

		movfw	IRLength
		movwf	Byte
		call	ByteStart

		movfw	RFAddress
		movwf	Byte
		call	ByteStart

SendingCycle
		movfw	INDF
		movwf	Byte
		call	ByteStart
		incf	FSR,1
		decfsz	LengthLoop, 1
		goto	SendingCycle
;il seguente ritardo e' relativo alla mancata consegna dei messaggi
;rilevata con un eccessivo ritardo nella ricezione del RFACK:
;per arrivare a campionare l'ack atteso può passare un tempo pari a:
;1540 + 1540 ms per l'attivazione della trasmissione da parte del destinatario del messaggio
;200+200+190 per doppio zero più 0x55
;190+190 per number e address
;190 per il check
;sono circa 4200 ms -> arrotondiamo ad un timeout di 5 ms
		
		movfw	TimeOutK
		movwf	TMR1H
		clrf	TMR1L
		TMR1En
		PEIEEn
		call	TxDisable
		sti
		return	


;;;;;;;Byte Sending Routine;;;;;		
ByteStart
    	bsf		Parity,0
    	movlw	8
		movwf	ByteCounter
		movfw	Byte
		movwf	ByteToSend
StartBit
		call	Zero
Invio	btfsc	ByteToSend,0
		goto	OneSend
ZeroSend
		call	Zero
		goto	ByteEnd
OneSend
		comf	Parity,1
		call	One
ByteEnd
		rrf		ByteToSend,1
		decfsz	ByteCounter,1
		goto	Invio
ParityBit
		btfsc	Parity,0
		goto	ParityOne
ParityZero
		call	Zero
		goto	StopBit
ParityOne
		call	One
StopBit
		call	One
		return




;;;;;;;Single Bit Send;;;;;;;;;;;
One
#ifndef	stampato
		bsf		PORTA,TXPIN
#else
		bsf		PORTC,TXPIN
#endif		
		goto	Durata
Zero
#ifndef	stampato
		bcf		PORTA,TXPIN
#else
		bcf		PORTC,TXPIN
#endif		

Durata
		movlw	12
		movwf	Count
		movlw	1
		movwf	Count+1
		call	SDelayLoop
		return


;;;;;;;;;;Preamble;;;;;;;;;;;;;;;
Intro
#ifndef	stampato
		bcf		PORTA,TXPIN
#else
		bcf		PORTC,TXPIN
#endif		

		movlw	180
		movwf	Count
		movlw	1
		movwf	Count+1
		call	SDelayLoop
#ifndef	stampato
		bsf		PORTA,TXPIN
#else
		bsf		PORTC,TXPIN
#endif		

		movlw	10
		movwf	Count
		movlw	1
		movwf	Count+1
		call	SDelayLoop
#ifndef	stampato
		bcf		PORTA,TXPIN
#else
		bcf		PORTC,TXPIN
#endif		

		movlw	180
		movwf	Count
		movlw	1
		movwf	Count+1
		call	SDelayLoop
#ifndef	stampato
		bsf		PORTA,TXPIN
#else
		bsf		PORTC,TXPIN
#endif		
		movlw	10
		movwf	Count
		movlw	1
		movwf	Count+1
		call	SDelayLoop
#ifndef	stampato
		bcf		PORTA,TXPIN
#else
		bcf		PORTC,TXPIN
#endif		
		return

;;;;;;;;RF [dis/en]abling Routines;;;;;;;
										;
										;
RxDisable								;
		btfsc	PORTC,RXENA				;
		return							;
		bsf		PORTC,RXENA				;
		clrf	Count					;
		movlw	6						;
		movwf	Count+1					;
		call	SDelayLoop				;
		return							;
										;
RxEnable								;
		btfss	PORTC,RXENA				;
		return							;
		bcf		PORTC,RXENA				;
		clrf	Count					;
		movlw	6						;
		movwf	Count+1					;
		call	SDelayLoop				;
		return							;
										;
TxDisable								;
#ifndef	stampato
		btfsc	PORTA,TXENA				;
		return
		bsf		PORTA,TXENA				;
#else
		btfsc	PORTC,TXENA				;
		return		
		bsf		PORTC,TXENA				;		
#endif		
										;
		clrf	Count					;
		movlw	6						;
		movwf	Count+1					;
		call	SDelayLoop				;
		return							;
										;
TxEnable								;
#ifndef	stampato
		btfss	PORTA,TXENA				;
		return							;										;
		bcf		PORTA,TXENA				;				
#else
		btfss	PORTC,TXENA				;
		return							;
		bcf		PORTC,TXENA				;
		
#endif		
		clrf	Count					;
		movlw	6						;
		movwf	Count+1					;
		call	SDelayLoop				;
		return							;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;DELAY ROUTINES;;;;;;;;;;;;;;;	
									;
;;;Every delay cycle lasts 1 us;;	;
RDelayLoop						;	;
		nop						;	;
		nop						;	;	
		decfsz	Count,1			;	;
		goto	RDelayLoop		;	;
		return					;	;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	

;;;Long Delay Routine;;;;;;;;;;;;	;
SDelayLoop						;	;
		nop						;	;
		nop						;	;
		decfsz	Count,1			;	;
		goto	SDelayLoop		;	;
								;	;
		decfsz	Count+1,1		;	;
		goto	SDelayLoop		;	;
								;	;
		return					;	;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;IRAck Routine;;;;;;;;;;;;;;;;;;;;
IRAckSend
		cli

		ContinueIRTX		
		
		movlw	0xff
		movwf	IRAckBuf
		
		movlw	0xf3
		movwf	IRAckBuf+1
		
		movlw	0xf3
		movwf	IRAckBuf+2

		movlw	3
		movwf	SIRLoop
		movlw	0x45
		movwf	IRFSR
		clrf	Complement
		banksel	TXSTA
		bsf		TXSTA,TX9D
		banksel	PORTA
		movlw	0x55
		movwf	TXREG


		IRRXDisable
		IRTXEnable
		PEIEEn
		sti
		return		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;RFAckSend Routine;;;;;;;;;;;;
RFAckSend

;IRAddress è [mittente, me]
		swapf	IRAddress,0		
		movwf	Sum
;to make check we have to increment sum because of the length of the message
		incf	Sum,1		
		incf	Sum,1

		call	RxDisable
		call	TxEnable
		cli
		call	Intro

	
		movlw	0x55
		movwf	Byte
		call	ByteStart
		
		movlw	B'00100010'
		movwf	Byte
		call	ByteStart
;IRAddr is in the swapped form we want ([sender,me])		
		swapf	IRAddress,0
		movwf	Byte
		call	ByteStart
		
		movfw	Sum
		movwf	Byte
		call	ByteStart

		call	TxDisable
		return
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;GiveTurn Routine;;;;;;;;;;;;;;;;;
GiveTurn
#ifndef stampato
#endif		

		cli
		btfsc	StatusByteRF,TURNTO
		goto	TurnProblem
;		movlw	TURNRETRIES
;		movwf	TurnRetries 
		movfw	MyAddr
		movwf	ToHaveTurn
		swapf	ToHaveTurn,1
		movfw	MyAddr
		movwf	SByte
		movfw	MaxAddr
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	TurnInc
TurnReset
		goto	TurnSend
TurnInc
		movfw	MyAddr
		addwf	ToHaveTurn,1
		incf	ToHaveTurn,1
		goto	TurnSend
TurnProblem
		bcf		StatusByteRF,TURNTO
;		decfsz	TurnRetries,1
;		goto	TurnSend
;		movlw	TURNRETRIES
;		movwf	TurnRetries 
		call	TurnIncr
		movfw	ToHaveTurn
		andlw	0x0f
		movwf	SByte
		movfw	MyAddr
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	TurnSend
		call	TurnIncr
TurnSend		
		movfw	ToHaveTurn
		movwf	Sum
		
; to make check we have to increment it by 2, i.e. the length of the message
		movlw	2
		addwf	Sum,1
		
		call	RxDisable
		call	TxEnable
		
		call	Intro
		
		movlw	0x55
		movwf	Byte
		call	ByteStart
		
		movlw	B'01000010'
		movwf	Byte
		call	ByteStart
		
		movfw	ToHaveTurn
		movwf	Byte
		call	ByteStart
		
		movfw	Sum
		movwf	Byte
		call	ByteStart
		
		bcf		StatusByteIRRF,TURN
		bcf		StatusByteRF,GOTTURN
; da quando inviamo il turno, sull'unità remota passano:
; - circa 1540 us per la RxDisable
; - circa 1540 us per la TxEnable		
; dopodichè può cominciare la sua trasmissione con i due start
; e il 55.  Alla ricezione del 55 l'unità che ha ceduto il turno
; può concludere che il turno è stato ricevuto. In tutto sono circa
; 3600-3700 us e abbondando sono 4000 che equivalgono a 20000 cicli
; istruzioni, pari a FFFF - b1df cicli --> si arrotonda a a500 che
; ha come effetto un'attesa di 23295 cicli pari a circa 4659 us
		
		movfw	TimeOutK
		movwf	TMR1H
		clrf	TMR1L
		cli
		TMR1En
		PEIEEn
		
		call	TxDisable
		sti
		return

TurnIncr	
		movfw	ToHaveTurn
		andlw	0x0f
		movwf	SByte
		movfw	MaxAddr
		movwf	RefByte
		call	Compare
		movwf	Result
		btfss	Result,0
		goto	TurnNRes
		movfw	ToHaveTurn
		andlw	0xf0
		movwf	ToHaveTurn
		goto	EndTurnIncr
TurnNRes
		movfw	ToHaveTurn
		addlw	1
		movwf	ToHaveTurn
EndTurnIncr
		return
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;Evaluation of Byte Parity;;;;
ParityEval						;
		movlw	8				;
		movwf	ParLoopCount	;
;Parity of 0 is 1 for Odd parity;
		clrf	Parity			;
		comf	Parity,1		;
ParityLoop						;
		btfsc	Byte,0			;	
		comf	Parity,1		;
		rrf		Byte,1			;
		decfsz	ParLoopCount,1	;
		goto	ParityLoop		;
		movfw	Parity			;
		andlw	H'01'			;
		movwf	Parity			;
		return					;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

IRTurnSend
		cli

		ContinueIRTX	
		
		movlw	0x20
		movwf	FSR

		movlw	0xa5
		movwf	INDF
		incf	FSR,1

		movlw	1
		movwf	SIRLoop
		movlw	0x20
		movwf	IRFSR
		clrf	Complement
		banksel	TXSTA
		bsf		TXSTA,TX9D
		banksel	PORTA
		movlw	0x55
		movwf	TXREG
		IRRXDisable
		IRTXEnable
		PEIEEn
		sti
		return		

IRBlockSend
		cli

		ContinueIRTX		

		bsf		StatusByteRES,RCXBLOCKED

		movlw	0x20
		movwf	FSR

		movlw	0x1e
		movwf	INDF
		incf	FSR,1

		movlw	1
		movwf	SIRLoop
		movlw	0x20
		movwf	IRFSR
		clrf	Complement
		banksel	TXSTA
		bsf		TXSTA,TX9D
		banksel	PORTA
		movlw	0x55
		movwf	TXREG
		IRRXDisable
		IRTXEnable
		PEIEEn
		sti
		
		return		


		END
 
;#if	(PR == 0) 
;		btfss	StatusByteRF,GOTTURN
;		goto	Nodebug
;;		btfss	StatusByteRF,RFMSGSENT
;		goto	Nodebug		
;		btfss	T1CON,TMR1ON
;;		goto	Debug
;		bsf		StatusByteIRRF,TMRWASON
;		goto	Nodebug
;Debug
;		btfss	StatusByteIRRF,TMRWASON
;		goto	DebugEnd	
;		bsf		PORTB,7
;		call	ViewDelay
;;		bcf		PORTB,7
;		call	ViewDelay
;		bsf		PORTB,7
;		call	ViewDelay
;		btfss	StatusByteIRRF,TMRWASDSBL
;		goto	DebugqEnd	
;		bsf		PORTB,7
;		goto	DebugEnd
;DebugqEnd
;		bcf		PORTB,7
;DebugEnd
;fermo	goto	fermo	
;Nodebug
;#endif
