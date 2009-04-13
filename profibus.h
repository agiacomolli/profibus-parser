/* 
 * File:   profibus.h
 * Author: Anderson A. Giacomolli
 *
 * Created on 14 de Abril de 2009, 19:39
 */

#ifndef _PROFIBUS_H
#define	_PROFIBUS_H

#ifdef	__cplusplus
extern "C" {
#endif

    //    PROFIBUS

    //    SD - Start Delimiter

    /*
#define SD_SEM_DADOS		0x10
#define	SD_TAMANHO_FIXO		0xA2
#define	SD_TAMANHO_VARIAVEL	0x68
#define SD_TOKEN		0xDC
     */

    typedef enum SD {
        SD_SEM_DADOS = 0x10,
        SD_TAMANHO_FIXO = 0xA2,
        SD_TAMANHO_VARIAVEL = 0x68,
        SD_TOKEN = 0xDC,
        SD_SC = 0xE5,
    } SD;

    //    FC - Frame Control
#define FC_RESERVADO		0x80
#define FC_FT			0x40
#define	FC_FCB			0x20
#define FC_FBV			0x10
#define FB_FUNCTION		0x0F

    //    Frame Control Function
    //    Station Type
#define ST_ESCRAVA		0x00
#define ST_MESTRE_NPRONTO_REDE	0x01
#define ST_MESTRE_PRONTO_REDE	0x02
#define ST_MESTRE_REDE		0x03

    //#define FT			0x20

    //    (bit6 = 1) (FT = 1) Envio / Requisição
#define FCF_SDN_L		0x04
#define FCF_SDN_H		0x06
#define FCF_DADOS_DIAG		0x07
#define FCF_STATUS_FDL_CONF	0x09
#define FCF_SRD_L		0x0C
#define FCF_SRD_H		0x0D
#define FCF_REQ_ID		0x0E
#define FCF_STATUS_LSAP		0x0F

    //    (bit6 = 0) (FT = 0) Resposta / Confirmação
#define FCF_ACK			0x00
#define FCF_NACK		0x01
#define FCF_RR			0x02
#define FCF_RS			0x03
#define FCF_DL			0x08
#define FCF_NR			0x09
#define FCF_DH			0x0A
#define FCF_RDL			0x0C
#define FCF_RDH			0x0D

    //    SA - Confirmação Curta
#define SHORT_ACK		0xE5

    //    SSAP - Source SAP
    //    DSAP - Destination SAP
    //    SAPs reservados pelo PROFIBUS - DP
#define SAP_WRITE_READ_DATA	DEFAULT
#define SAP_MM_COMM		0x36
#define SAP_SET_SLAVE_ADD	0x37
#define SAP_RD_INP		0x38
#define SAP_RD_OUT		0x39
#define SAP_GLOBAL_CONTROL	0x3A
#define SAP_GET_CFG		0x3B
#define SAP_SLAVE_DIAGNOSIS	0x3C
#define SAP_SET_PRM		0x3D
#define SAP_CHK_CFG		0x3F

    //    ED - End Delimiter
    //#define ED			0x16

    typedef struct frameControl_bit {
        unsigned FCF : 4;
        unsigned FBV : 1;
        unsigned FCB : 1;
        unsigned FT : 1;
        unsigned reserv : 1;
    } frameControl_bit;

    typedef struct oct {
        unsigned bit0 : 1;
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
    } oct;

    typedef union octeto{
        oct bit;
        char byte;
    }octeto;

    typedef union frameControl {
        frameControl_bit bit;
        char byte;
    } frameControl;

    typedef struct frame {
        unsigned char type;
        unsigned char LE;
        unsigned char LEr;
        unsigned char SD;
        unsigned char DA;
        unsigned char SA;
        //unsigned char FC;
        frameControl FC;
        unsigned char DSAP;
        unsigned char SSAP;
        unsigned char *DU;
        unsigned char CKS;
        unsigned char ED;
        unsigned char L;
        octeto oc;
        //union
    } frame;

    extern int atohex(const char *_nptr);

    //extern int getFrame(FILE *fIn, frame *f);
    extern int getFrame(SD type, FILE *fIn, FILE * fOut);

    extern unsigned char getDado(FILE *_file);

    extern char *frameMsg(int frameType, int function);


#ifdef	__cplusplus
}
#endif

#endif	/* _PROFIBUS_H */

