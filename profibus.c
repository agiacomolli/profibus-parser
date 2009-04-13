#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "profibus.h"

int atohex(const char *_nptr) {
    int tmp = 0, i, j = 0;

    for (i = 0; *_nptr != 0; i++, _nptr++);
    _nptr -= i;

    for (i--; i >= 0; i--, _nptr++) {
        if (*_nptr >= 'A' && *_nptr <= 'F') tmp += (*(_nptr) - 55) * (int) (pow(16, i));
            //else if (*_nptr >= 'a' && *_nptr <= 'f') tmp += (*(_nptr) - 87) * (int) (pow(16, i));
        else if (*_nptr >= '0' && *_nptr <= '9') tmp += (*(_nptr) - 48) * (int) (pow(16, i));
    }
    return tmp;
}

unsigned char getDado(FILE *_file) {
    char buf[10] = "\0";
    char tmp = '\0';
    int i = 0;
    if ((tmp = fgetc(_file)) != EOF) {
        while ((tmp >= 'A' && tmp <= 'F') || (tmp >= '0' && tmp <= '9')) {
            buf[i] = tmp;
            buf[++i] = '\0';
            if ((tmp = fgetc(_file)) == EOF) return EOF;
        }
        return atohex(buf);
    } else return EOF;
}

/*
void frameMsg(int frameType, int function, FILE *_file) {
    if (frameType) {
        switch (function) {
            case FCF_SDN_L:
                fputs("SDN baixa prioridade.\n", _file);
                break;
            case FCF_SDN_H:
                fputs("SDN alta prioridade.\n", _file);
                break;
            case FCF_DADOS_DIAG:
                fputs("Requisicao de dados de diagnostico.\n", _file);
                break;
            case FCF_STATUS_FDL_CONF:
                fputs("Requisicao de status de FDL com confirmacao.\n", _file);
                break;
            case FCF_SRD_L:
                fputs("SRD baixa prioridade.\n", _file);
                break;
            case FCF_SRD_H:
                fputs("SRD alta prioridade.\n", _file);
                break;
            case FCF_REQ_ID:
                fputs("Requisicao do ID na resposta.\n", _file);
                break;
            case FCF_STATUS_LSAP:
                fputs("Requisicao do status do LSAP na resposta.\n", _file);
                break;
            default:
                fputs("Erro frameMsg.\n", _file);
                break;
        }
    } else {
        switch (function) {
            case FCF_ACK:
                fputs("Confirmacao positiva.\n", _file);
                break;
            case FCF_NACK:
                fputs("Confirmacao negativa.\n", _file);
                break;
            case FCF_RR:
                fputs("Sem recursos para transmitir dados.\n", _file);
                break;
            case FCF_RS:
                fputs("Servidor nao-ativado (RS). SAP nao-ativado.\n", _file);
                break;
            case FCF_DL:
                fputs("Transmissao de dados ok. Baixa prioridade.\n", _file);
                break;
            case FCF_NR:
                fputs("Sem resposta de dados de FDL/FMA 1/2.\n", _file);
                break;
            case FCF_DH:
                fputs("Transmissao de dados ok. Alta prioridade.\n", _file);
                break;
            case FCF_RDL:
                fputs("Incapaz de transmitir baixa prioridade.\n", _file);
                break;
            case FCF_RDH:
                fputs("Incapaz de transmitir alta prioridade.\n", _file);
                break;

            default:
                fputs("Erro frameMsg.\n", _file);
                break;
        }
    }
}
 */
char *frameMsg(int frameType, int function) {
    if (frameType) {
        switch (function) {
            case FCF_SDN_L:
                return "SDN baixa prioridade";
                break;
            case FCF_SDN_H:
                return "SDN alta prioridade";
                break;
            case FCF_DADOS_DIAG:
                return "requisicao de dados de diagnostico (mestre)";
                break;
            case FCF_STATUS_FDL_CONF:
                return "requisicao de status de FDL com confirmacao (mestre)";
                break;
            case FCF_SRD_L:
                return "SRD baixa prioridade";
                break;
            case FCF_SRD_H:
                return "SRD alta prioridade";
                break;
            case FCF_REQ_ID:
                return "requisicao do ID na resposta (mestre)";
                break;
            case FCF_STATUS_LSAP:
                return "requisicao do status do LSAP na resposta (mestre)";
                break;
            default:
                return "erro frameMsg";
                break;
        }
    } else {
        switch (function) {
            case FCF_ACK:
                return "confirmacao positiva (escravo)";
                break;
            case FCF_NACK:
                return "confirmacao negativa (escravo)";
                break;
            case FCF_RR:
                return "sem recursos para transmitir dados (escravo)";
                break;
            case FCF_RS:
                return "servidor nao-ativado (RS). SAP nao-ativado";
                break;
            case FCF_DL:
                return "transmissao de dados ok. Baixa prioridade";
                break;
            case FCF_NR:
                return "sem resposta de dados de FDL/FMA 1/2 (escravo)";
                break;
            case FCF_DH:
                return "transmissao de dados ok. Alta prioridade";
                break;
            case FCF_RDL:
                return "incapaz de transmitir baixa prioridade (escravo)";
                break;
            case FCF_RDH:
                return "incapaz de transmitir alta prioridade (escravo)";
                break;

            default:
                return "erro frameMsg";
                break;
        }
    }
}

/*
int getFrame(FILE *fIn, frame *f) {
    int i, j;
    char type;
    unsigned char tmp;

    memset(f, 0, sizeof (frame));
    if ((type = getDado(fIn)) != EOF) {
        i = j = tmp = 0;
        switch (type) {
            case SD_SEM_DADOS:
                f->type = SD_SEM_DADOS;
                f->DA = getDado(fIn);
                f->SA = getDado(fIn);
                f->FC.byte = getDado(fIn);
                f->CKS = getDado(fIn);
                f->ED = getDado(fIn);
                break;
            case SD_TAMANHO_FIXO:
                f->type = SD_TAMANHO_FIXO;
                f->DU = malloc(sizeof (char) * 8);
                f->DA = getDado(fIn);
                f->SA = getDado(fIn);
                f->FC.byte = getDado(fIn);
                f->CKS = getDado(fIn);
                f->ED = getDado(fIn);
                break;
            case SD_TAMANHO_VARIAVEL:
                f->type = SD_TAMANHO_VARIAVEL;
                f->LE = getDado(fIn);
                f->LEr = getDado(fIn);
                f->SD = getDado(fIn);
                f->DA = getDado(fIn);
                f->SA = getDado(fIn);
                f->FC.byte = getDado(fIn);
                tmp += f->DA + f->SA + f->FC.byte;
                i = f->LE - 3;
                f->DU = malloc(sizeof (char) * i);
                for (i, j = 0; j < i; j++) {
                    f->DU[j] = getDado(fIn);
                    tmp += f->DU[j];
                }
                f->CKS = getDado(fIn);
                f->ED = getDado(fIn);
                break;
            case SD_TOKEN:
                f->type = SD_TOKEN;
                f->DA = getDado(fIn);
                f->SA = getDado(fIn);
                break;
            case SD_SA:
                f->type = SD_SA;
        }
        //printf("type %X\n", type);
        return 1;
    } else {
        //printf("FIM\n");
        return 0;
    }
}
 */
void printFrame(frame *f) {
    switch (f->type) {
        case SD_SEM_DADOS:
            break;
        case SD_TAMANHO_FIXO:
            break;
        case SD_TAMANHO_VARIAVEL:
            break;
        case SD_TOKEN:
            printf("\nPassagem de token.\n");
            printf("Start delimiter %X\n", f->SD);
            printf("Destination address %X\n", f->DA);
            printf("Source address %X\n", f->SA);
            break;
    }
}

int getFrame(SD type, FILE *fIn, FILE * fOut) {
    int i, j;
    unsigned char byte, byte_tmp;
    static int frameCount = 0;
    frame f;

    memset(&f, 0, sizeof (f));
    f.FC.byte = 0;
    byte = byte_tmp = i = j = 0;

    switch (type) {
        case SD_SEM_DADOS:
            f.type = SD_SEM_DADOS;
            f.DA = getDado(fIn);
            f.SA = getDado(fIn);
            f.FC.byte = getDado(fIn);
            f.CKS = getDado(fIn);
            f.ED = getDado(fIn);

            fprintf(fOut, "\nFRAME #%d", ++frameCount);
            fprintf(fOut, "\n%X %X %X %X %X %X %X", f.type, f.DA, f.SA, f.FC.byte, f.CKS, f.ED, f.ED);
            fputs("\nMensagem sem dados (controle).\n", fOut);

            if ((f.ED) != 0x16) {
                fputs("Fim inesperado de mensagem.\n", fOut);
            }
            if (f.CKS != (f.DA + f.SA + f.FC.byte)) {
                fputs("Checksum errado.\n", fOut);
            }
            if (f.DA > 0x7F || f.SA > 0x7F) {
                fputs("Fora da faixa de enderecos.\n", fOut);
            }
            if (f.DA == 0x7F) {
                fputs("DA broadcast.\n", fOut);
            }
            if (f.SA == 0x7F) {
                fputs("SA broadcast.\n", fOut);
            }

            if (f.FC.bit.FT) fputs("Requisicao de mensagem.\n", fOut);
            else fputs("Resposta de mensagem.\n", fOut);

            if (f.FC.bit.FCB) fputs("Frame Count Bit 1.\n", fOut);
            else fputs("Frame Count Bit 0.\n", fOut);

            if (f.FC.bit.FBV) fputs("Funcao FCB valida. Alterna valores de FCB.\n", fOut);
            else fputs("Funcao FCB invalida. Nao alterna valores de FCB.\n", fOut);

            //fprintf(fOut, "VALOR FC: %x, DESL %x\n", f.FC.bit.FCF, f.FC.bit.FCF >> 2);
            /*
            switch (f.FC.bit.FCF >> 2) {
                case ST_ESCRAVA:
                    fputs("Estacao escrava.\n", fOut);
                    break;
                case ST_MESTRE_NPRONTO_REDE:
                    fputs("Mestre nao-pronto a entrar na rede.\n", fOut);
                    break;
                case ST_MESTRE_PRONTO_REDE:
                    fputs("Mestre pronto para entrar na rede.\n", fOut);
                    break;
                case ST_MESTRE_REDE:
                    fputs("Mestre na rede.\n", fOut);
                    break;
                default:
                    fprintf(fOut, "Erro: %x\n", f.FC.bit.FCF);
                    break;
            }
             */
            //frameMsg(f.FC.bit.FT, f.FC.bit.FCF);

            fprintf(fOut, "Mensagem de %s sem dados da estacao %Xh para a estacao %Xh\n",
                    frameMsg(f.FC.bit.FT, f.FC.bit.FCF), f.SA, f.DA);

            break;
        case SD_TAMANHO_FIXO:
            f.type = SD_TAMANHO_FIXO;
            f.DU = malloc(sizeof (char) * 8);
            f.DA = getDado(fIn);
            f.SA = getDado(fIn);
            f.FC.byte = getDado(fIn);
            f.CKS = getDado(fIn);
            f.ED = getDado(fIn);

            fprintf(fOut, "\nFRAME #%d", ++frameCount);
            fprintf(fOut, "\n%X %X %X %X %X %X", f.type, f.DU, f.DA, f.SA, f.FC.byte, f.CKS, f.ED);
            fputs("\nMensagem de tamanho fixo.\n", fOut);
            if ((f.ED) != 0x16) {
                fputs("Fim inesperado de mensagem.\n", fOut);
            }
            if (f.CKS != (f.DA + f.SA + f.FC.byte)) {
                fputs("Checksum errado.\n", fOut);
            }

            fputs("Mensagem: ", fOut);
            for (i = 0; i < 8; i++) {
                fprintf(fOut, "%X ", f.DU[i]);
            }
            fputc('\n', fOut);

            break;

        case SD_TAMANHO_VARIAVEL:
            f.type = SD_TAMANHO_VARIAVEL;
            f.LE = getDado(fIn);
            f.LEr = getDado(fIn);
            f.SD = getDado(fIn);
            f.DA = getDado(fIn);
            f.SA = getDado(fIn);
            f.FC.byte = getDado(fIn);

            byte_tmp += f.DA + f.SA + f.FC.byte;

            i = f.LE - 3;

            if (f.DA >> 7) {
                f.DSAP = getDado(fIn);
                byte_tmp += f.DSAP;
                i--;
                f.L = 1;
            }
            if (f.SA >> 7) {
                f.SSAP = getDado(fIn);
                byte_tmp += f.SSAP;
                i--;
                f.L = 1;
            }

            f.DU = malloc(sizeof (char) * i);
            for (i, j = 0; j < i; j++) {
                f.DU[j] = getDado(fIn);
                byte_tmp += f.DU[j];
            }
            //f.DU[j] = '\0';

            f.CKS = getDado(fIn);
            f.ED = getDado(fIn);

            fprintf(fOut, "\nFRAME #%d", ++frameCount);
            fprintf(fOut, "\n%X %X %X %X %X %X %X ", f.type, f.LE, f.LEr, f.SD, f.DA, f.SA, f.FC.byte);
            for (i, j = 0; j < i; j++) fprintf(fOut, "%X ", f.DU[j]);
            fprintf(fOut, "%X %X", f.CKS, f.ED);

            fputs("\nMensagem de tamanho variavel.\n", fOut);
            fprintf(fOut, "Tamanho da mensagem %Xh\n", f.LE);
            fprintf(fOut, "Mensagem de %s da estacao %Xh (%s) para a estacao %Xh (%s).\n", frameMsg(f.FC.bit.FT, f.FC.bit.FCF), f.SA, (f.FC.bit.FT) ? "mestre" : "escravo", f.DA, (f.FC.bit.FT) ? "escravo" : "mestre");

            if (f.L) {
                if (f.DSAP == 0x3C && f.SSAP == 0x3E) {
                    fputs("Telegrama de requisicao de diagnostico.\n", fOut);
                } else if (f.DSAP == 0x3E && f.SSAP == 0x3C) {
                    fputs("Telegrama de resposta a requisicao de diagnostico.\n", fOut);
                    f.oc.byte = f.DU[0];
                    fprintf(fOut, "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s", f.oc.bit.bit0 ? "Estacao inexistente\n." : "Estacao existente.\n",
                            f.oc.bit.bit1 ? "Estacao nao-pronta para troca de dados.\n" : "Estacao pronta para troca de dados.\n",
                            f.oc.bit.bit2 ? "Falha na configuracao. Os dados de configuracao nao coincidem.\n" : "Configuracao correta.\n",
                            f.oc.bit.bit3 ? "Diagnostico externo.\n" : "",
                            f.oc.bit.bit4 ? "Nao suportado. O servico requisitado nao e suportado pelo escravo.\n" : "",
                            f.oc.bit.bit5 ? "Resposta do escravo invalida.\n" : "",
                            f.oc.bit.bit6 ? "Falha na parametrizacao.\n" : "",
                            f.oc.bit.bit7 ? "Trancado pelo mestre.\n" : "");
                    f.oc.byte = f.DU[1];
                    fprintf(fOut, "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s"
                            "%s", f.oc.bit.bit0 ? "" : "Requisicao de parametrizacao. O escravo tem de ser parametrizado.\n",
                            f.oc.bit.bit1 ? "Diagnostico estatico.\n" : "",
                            f.oc.bit.bit2 ? "" : "",
                            f.oc.bit.bit3 ? "Habilitacao do watchdog.\n" : "",
                            f.oc.bit.bit4 ? "Comando FREEZE foi recebido.\n" : "",
                            f.oc.bit.bit5 ? "Comando SYNC foi recebido.\n" : "",
                            f.oc.bit.bit6 ? "" : "",
                            f.oc.bit.bit7 ? "" : "");
                    f.oc.byte = f.DU[2];
                    fprintf(fOut, "%s", f.oc.bit.bit7 ? "Estouro de diagnostico externo.\n" : "");
                    f.oc.byte = f.DU[3];
                    fprintf(fOut, "%s", f.oc.byte == 0xFF ? "Escravo nao-parametrizado.\n" : "");

                } else if (f.DSAP == 0x3D && f.SSAP == 0x3E) {
                    fputs("Telegrama de parametrizacao.\n", fOut);
                    f.oc.byte = f.DU[0];
                    fprintf(fOut, "%s"
                            "%s",
                            f.oc.bit.bit3 ? "Habilita watchdog.\n" : "Desabilita watchdog.\n",
                            f.oc.bit.bit4 ? "Escravo requisitado para trabalhar em modo FREEZE.\n" : "",
                            f.oc.bit.bit5 ? "Escravo requisitado para operar em modo SYNC.\n" : "");
                } else if (f.DSAP == 0x3E && f.SSAP == 0x3E) {
                    fputs("Telegrama de configuracao.\n", fOut);
                } else if (f.DSAP == 0x38 && f.SSAP == 0x3E) {
                    fputs("Comunicacao de dados (READ_INPUT).\n", fOut);
                } else if (f.DSAP == 0x39 && f.SSAP == 0x3E) {
                    fputs("Comunicacao de dados (READ_OUTPUT).\n", fOut);
                } else if (f.DSAP == 0x3E && f.SSAP == 0x3A) {
                    fputs("Servicos de controle global.\n", fOut);
                    f.oc.byte = f.DU[0];
                    fprintf(fOut, "%s"
                            "%s",
                            "%s",
                            "%s",
                            "%s",
                            f.oc.bit.bit1 ? "CLEAR.\n" : "",
                            f.oc.bit.bit2 ? "UNFREEZE.\n" : "",
                            f.oc.bit.bit3 ? "FREEZE.\n" : "",
                            f.oc.bit.bit4 ? "UNSYNC.\n" : "",
                            f.oc.bit.bit5 ? "SYNC.\n" : "");
                }

            }
            if ((f.ED) != 0x16) {
                fputs("Fim inesperado de mensagem.\n", fOut);
            }
            if (f.LE != f.LEr) {
                fputs("Tamanhos das mensagens diferentes (LE != LEr).\n", fOut);
            }

            break;
        case SD_TOKEN:
            f.type = SD_TOKEN;
            f.DA = getDado(fIn);
            f.SA = getDado(fIn);
            fprintf(fOut, "\nFRAME #%d", ++frameCount);
            fprintf(fOut, "\n%X %X %X", f.type, f.DA, f.SA);
            fprintf(fOut, "\nPassagem de token do mestre (%Xh) para %Xh.\n", f.SA, f.DA);
            break;
        case SD_SC:
            f.type = SD_SC;
            fprintf(fOut, "\nFrame #%d", ++frameCount);
            fputs("\nMensagem de confirmacao curta.\n", fOut);
            break;
    }
}

