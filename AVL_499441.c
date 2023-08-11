#include <stdio.h>
#include <stdlib.h>

// Aluno: Jailon William
// Matrícula: 499441

typedef struct no{
    int valor;
    int bal;
    struct no *dir;
    struct no *esq;
}NO;

// global
NO *ptRaiz = NULL; // Criando a Raiz da AVL
int h = 0; // 1 se altura foi modificada ou 0 caso contrário.

// Funções - Inserir
void Caso_1(NO **pt, int *h);
void Caso_2(NO **pt, int *h);
int InserirAVL(int x, NO **pt, int *h);

// Funções - Remover
void RemoverAVL(int x, NO **pt, int *h);
void Caso_1R(NO **pt, int *h);
void Caso_2R(NO **pt, int *h);
void balancear(NO **pt, char R, int *h);
void trocar(NO **pt_2, NO **PaiSsqd);

// Função - Buscar
NO *Buscar(int x, NO *pt);

// Função - Para contar quantos nós a AVL tem
int ContaNO(NO *pt);

// função imprimir
int PreOrdem(NO *pt);

// Funções - VerificadorAVL
int VerificadorAVL(NO *pt);
int AlturaNO(NO *pt);
int Bal(NO *pt);

// Funções - Testes
void teste(NO **pt, int *h, int qtd_avl, int qtd_no_inserir, int qtd_no_remover);
void LimparAVL(NO **pt);

int main(){
    NO **pt = &ptRaiz; // Ponteiro que ira percorrer a arvore

    int inserir_teste[13] = {10, 15, 16, 8, 6, 9, 12, 18, 11, 13, 14, 17, 19};

    printf("\n################################## TESTE DO CASO PEQUENO #####################################\n");
    for (int i = 0; i <= 12; i++){
        InserirAVL(inserir_teste[i], pt, &h);
        printf("\n------------------------------------------------------\n");
        PreOrdem(ptRaiz);
    }

    RemoverAVL(11, &ptRaiz, &h);
    PreOrdem(ptRaiz);
    RemoverAVL(8, &ptRaiz, &h);
    PreOrdem(ptRaiz);
    
    printf("\n################################## TESTE DO INSERIR #####################################\n");
    teste(pt, &h, 1000, 10000, 0); // Teste o inserir, 1000 AVL inserindo 10000 nós aleatórios e removendo 0.
    
    printf("\n################################## TESTE DO REMOVER #####################################\n");
    teste(pt, &h, 1000, 10000, 1000); // teste o remover, 1000 AVL inserindo 10000 nós aleatórios e removendo 1000.

    return 0;
}

int InserirAVL(int x, NO **pt, int *h){
    if ((*pt) == NULL){
        NO *novo = malloc(sizeof(NO)); // alocando memória pro nó
        
        // setando os valores
        novo->valor = x; 
        novo->bal = 0;
        novo->dir = NULL;
        novo->esq = NULL;

        // inserindo na AVL
        (*pt) = novo;
        *h = 1;

    }else{
        if (x == (*pt)->valor){
            // printf("Valor encontrado!"); // AVL não aceita elementos repetidos.
            *h = 0; // Como não teve alteração na AVL, h é falso.
            
        }else{
            if(x < (*pt)->valor){ // elemento está na esquerda desse nó
                InserirAVL(x, &(*pt)->esq, h);
                
                if (*h == 1){ // se altura foi modificada.
                    switch ((*pt)->bal){
                        case 1:
                            (*pt)->bal = 0;
                            *h = 0;
                            break;
                        
                        case 0:
                            (*pt)->bal = -1;
                            break;

                        case -1:
                            Caso_1(pt, h);
                            break;

                        default:
                            break;
                    }
                }

            }else{ // elemento esta a direita
                InserirAVL(x, &(*pt)->dir, h);
                
                if (*h == 1){ // se altura foi modificada.
                    switch ((*pt)->bal){
                        case -1:
                            (*pt)->bal = 0;
                            *h = 0;
                            break;
                        
                        case 0:
                            (*pt)->bal = 1;
                            break;

                        case 1:
                            Caso_2(pt, h);
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }
}

void Caso_1(NO **pt, int *h){ // rotação para direita
    NO *ptu = (*pt)->esq; // ponteiro para um nó

    if (ptu->bal == -1){ // Caso simples

        // alterando ponteiros
        (*pt)->esq = ptu->dir; 
        ptu->dir = (*pt); 
        (*pt) = ptu; // Atualizando o pt

        (*pt)->dir->bal = 0;

    }else{ // Caso duplo
        NO *ptv = ptu->dir; 

        // alterando ponteiros
        ptu->dir = ptv->esq; 
        ptv->esq = ptu; 
        (*pt)->esq = ptv->dir; 
        ptv->dir = (*pt); 

        // ajustando bal
        if(ptv->bal == 1){
            if (ptu->esq == NULL && ptu->dir == NULL){ // Caso seja folha
                ptu->bal = 0;

            }else{
                ptu->bal = -1;

            }
            (*pt)->bal = 0;

        }else{
            ptu->bal = 0;
            if((*pt)->esq == NULL && (*pt)->dir == NULL){ // Caso seja folha
                (*pt)->bal = 0;

            }else{
                (*pt)->bal = 1;

            }
        }
        (*pt) = ptv; // atualizando o pt. 
    }

    (*pt)->bal = 0; 
    *h = 0;
}

void Caso_2(NO **pt, int *h){ // rotação para esquerda
    NO *ptu = (*pt)->dir; // ponteiro para um nó

    if (ptu->bal == 1){ // Caso simples

        // alterando ponteiros
        (*pt)->dir = ptu->esq; 
        ptu->esq = (*pt); 
        (*pt) = ptu; // atualizando o pt.
        (*pt)->esq->bal = 0;

    }else{ // Caso duplo
        NO *ptv = ptu->esq;

        // alterando ponteiros
        (*pt)->dir = ptv->esq;
        ptv->esq = (*pt);
        ptu->esq = ptv->dir;
        ptv->dir = ptu;

        // ajustando bal
        if(ptv->bal == 1){
            ptu->bal = 0;
            if((*pt)->esq == NULL && (*pt)->dir == NULL){ // Caso seja uma folha
                (*pt)->bal = 0;

            }else{
                (*pt)->bal = -1;

            }

        }else{
            if (ptu->dir == NULL && ptu->esq == NULL){ // Caso seja uma folha
                ptu->bal = 0;

            }else{
                ptu->bal = 1;

            }
            (*pt)->bal = 0;

        }
        (*pt) = &(*ptv); // atualizando o pt.
    }

    (*pt)->bal = 0; // ajustando bal da raiz
    *h = 0; // Recebe falso, pois não alterou o tamanho da raiz
}

void RemoverAVL(int x, NO **pt, int *h){
    if ((*pt) == NULL){
        printf("Não existe esse no");
        *h = 0; // Falso, pois não ouve alterações.

    }else{
        if (x < (*pt)->valor){ // nó esta a esquerda
            RemoverAVL(x, &(*pt)->esq, h);
            balancear(&(*pt), 'E', h);

        }else{
            if (x > (*pt)->valor){ // nó esta a direita
                RemoverAVL(x, &(*pt)->dir, h);
                balancear(&(*pt), 'D', h);
                
            }else{
                NO *aux = (*pt);

                if ((*pt)->esq == NULL){ 
                    (*pt) = (*pt)->dir;
                    *h = 1;
                    free(aux);

                }else{
                    if ((*pt)->dir == NULL){
                        (*pt) = (*pt)->esq;
                        *h = 1;
                        free(aux);

                    }else{
                        NO *s = (*pt)->dir;

                        if (s->esq == NULL){
                            (*pt)->dir = s->dir;
                            s->esq = (*pt)->esq;
                            s->bal = (*pt)->bal;
                            (*pt) = s;

                            *h = 1;
                            free(aux);
                        }else{
                            NO *PaiS = s;

                            while (s->esq != NULL){
                                PaiS = s;
                                s = s->esq;
                            }

                            trocar(pt, &(PaiS->esq));
                            RemoverAVL(x, &(*pt)->dir, h);
                        }
                        balancear(pt, 'D', h);
                    }
                }
            }
        }
    }
}

void Caso_1R(NO **pt, int *h){
    NO *ptu = (*pt)->esq;
    
    if (ptu->bal <= 0){ // Caso simples

        // alterando ponteiros
        (*pt)->esq = ptu->dir; 
        ptu->dir = (*pt); 
        (*pt) = ptu;  

        // ajustando bal
        if (ptu->bal == -1){
            ptu->bal = (*pt)->dir->bal = 0;
            *h = 1;

        }else{
            ptu->bal = 1;
            (*pt)->dir->bal = -1;
            *h = 0;
        }
    }else{ // Caso duplo
        NO *ptv = ptu->dir; 

        // alterando ponteiros
        ptu->dir = ptv->esq; 
        ptv->esq = ptu; 
        (*pt)->esq = ptv->dir; 
        ptv->dir = (*pt); 

        switch (ptv->bal){ // ajustando bal
            case -1:
                ptu->bal = 0;

                if ((*pt)->dir == NULL && (*pt)->esq == NULL){
                    (*pt)->bal = 0;
                }else{
                    (*pt)->bal = 1;
                }
                
                break;
            
            case 0:
                ptu->bal = 0;
                (*pt)->bal = 0;
                break;

            case 1:
                if (ptu->esq == NULL && ptu->dir == NULL){
                    ptu->bal = 0;
                }else{
                    ptu->bal = -1;
                }

                (*pt)->bal = 0;
                break;

            default:
                break;
        } 
        
        (*pt) = ptv; // atualizando pt
        (*pt)->bal = 0;
        *h = 1; 
    }
}

void Caso_2R(NO **pt, int *h){
    NO *ptu = (*pt)->dir;
    
    if (ptu->bal >= 0){ // Caso simples

        // ajustando ponteiros
        (*pt)->dir = ptu->esq; 
        ptu->esq = (*pt); 
        (*pt) = ptu;  

        // Atualizando bal
        if (ptu->bal == 1){
            ptu->bal = (*pt)->esq->bal = 0;
            *h = 1;

        }else{
            ptu->bal = -1;
            (*pt)->esq->bal = 1;
            *h = 0;

        }
    }else{ // Caso duplo
        NO *ptv = ptu->esq; 

        // ajustando ponteiros
        ptu->esq = ptv->dir; 
        ptv->dir = ptu; 
        (*pt)->dir = ptv->esq; 
        ptv->esq = (*pt); 

        switch (ptv->bal){ // ajustando bal
            case -1:
                if (ptu->esq == NULL && ptu->dir == NULL){
                    ptu->bal = 0;

                }else{
                    ptu->bal = 1;
                }

                (*pt)->bal = 0;
                break;
            
            case 0:
                ptu->bal = 0;
                (*pt)->bal = 0;
                break;

            case 1:
                ptu->bal = 0;

                if ((*pt)->dir == NULL && (*pt)->esq == NULL){
                    (*pt)->bal = 0;
                }else{
                    (*pt)->bal = -1;
                }

                break;

            default:
                break;
        }
                
        (*pt) = ptv; // atualizando ponteiro
        (*pt)->bal = 0;
        *h = 1; 
    }
}

void balancear(NO **pt, char R, int *h){
    if (*h == 1){
        if (R == 'D'){ // Balancear para direita
            switch ((*pt)->bal){
                case 1:
                    (*pt)->bal = 0;
                    break;
                
                case 0:
                    (*pt)->bal = -1;
                    *h = 0;
                    break;

                case -1:
                    Caso_1R(pt, h);
                    break;

                default:
                    break;
            }
        }else{ // Balancear pra esquerda
            switch ((*pt)->bal){
                case -1:
                    (*pt)->bal = 0;
                    break;
                
                case 0:
                    (*pt)->bal = 1;
                    *h = 0;
                    break;

                case 1:
                    Caso_2R(pt, h);
                    break;

                default:
                    break;
            }
        }
    }
}

void trocar(NO **no_1, NO **no_2){
    int aux = (*no_1)->valor;
    (*no_1)->valor = (*no_2)->valor;
    (*no_2)->valor = aux;
}

int AlturaNO(NO *pt){
    if (pt != NULL){
        int alt_esq = AlturaNO(pt->esq);
        int alt_dir = AlturaNO(pt->dir); 

        // eh acrescentado +1 as demais operações pois é considerado o próprio nó.
        if (alt_dir > alt_esq){ 
            return alt_dir + 1; 

        }else{
            if (alt_dir < alt_esq){ 
                return alt_esq + 1;
            
            }else{
                return alt_esq + 1;
            
            }
        }

    }else{
        return 0;
    }
}

int ContaNO(NO *pt){
    if (pt != NULL){ // Conta quantos nós tem a esquerda e quantos nós tem a direita + o proprio nó.
        return 1 + ContaNO(pt->dir) + ContaNO(pt->esq);

    }else{
        return 0;

    }
}

int VerificadorAVL(NO *pt){
    if (pt != NULL){
        // Verificando se o lado esquerdo e direito é AVL. Pois, caso não sejam, é impossível a árvore ser AVL.
        int esq_teste = VerificadorAVL(pt->esq);
        int dir_teste = VerificadorAVL(pt->dir);
        int bal_no; 

        if (esq_teste == 0 || dir_teste == 0){ // Se qualquer um dos dois lados não forem AVL.
            return 0;                          // Logo, a arvore não é AVL.
        }

        if (pt != NULL){
            bal_no = (AlturaNO(pt->dir) - AlturaNO(pt->esq)); // Recalculando bal para comparação

        }else{
            bal_no = 0;
        }

        // pode ocorrer do bal do nó ser diferente do bal esperado e estar entre -1 e 1.
        if ((bal_no != pt->bal) && (bal_no > 1 || bal_no < -1)){ // Caso ocorra, não é AVL
            return 0;
        }
        return 1;
    }
    else{
        return 1;

    }

}

void teste(NO **pt, int *h, int qtd_avl, int qtd_no_inserir, int qtd_no_remover){
    int cont = 0;
    
    for (int j = 0; j < qtd_avl; j++){
        while (ContaNO(ptRaiz) != qtd_no_inserir){
            int num = rand() % 100000;
            InserirAVL(num, pt, h);
        }
        
        
        // for (int i = 0; i < qtd_no_inserir; i++){ // inserindo os nós na AVL
        //     int num = rand() % 100000;  
        //     InserirAVL(num, pt, h);
            
        // }
        
        printf("\n-----------------------------------------------------------------------------------------\n");
        printf("A AVL %d: foi inserido %d", j+1, ContaNO(ptRaiz));

        for (int k = 0; k != qtd_no_remover; k++){// removendo 1000 nos
            RemoverAVL(ptRaiz->valor, &ptRaiz, h);
        }
        

        if (VerificadorAVL(ptRaiz) == 1){ // verificando se ela é AVL
            printf(", apos a remocao, arvore contem %d. Altura: %d. Arvore continua sendo uma AVL.\n\n", ContaNO(ptRaiz), AlturaNO(ptRaiz));
        
        }else{
            printf(", apos a remocao, arvore contem %d. ARVORE NAO EH AVL!!!\n\n", ContaNO(ptRaiz));
        
        }

        LimparAVL(pt); // Limpando a arvore para recomeçar
    }
}

NO *Buscar(int x, NO *pt){
    while(pt != NULL){
      if (x == pt->valor){ // achamos o x na AVL.
        return pt; 
      }else{
        if (x < pt->valor){ // x está a esquerda
          pt = pt->esq;
        }else{ // x está a direita
          pt = pt->dir;
        }
      }
    }

    return NULL; // o elemento não existe nessa árvore
}

void LimparAVL(NO **pt){
    if ((*pt) != NULL){
        if ((*pt)->esq != NULL){ // se existe um nó a esquerda, chamamos a função LimparAVL.
            NO *aux = (*pt)->esq;
            LimparAVL(&aux);
        }
        if ((*pt)->dir != NULL){ // se existe um nó a direita, chamamos a função LimparAVL.
            NO *aux = (*pt)->dir; 
            LimparAVL(&aux);
        }
        
        // quando pt for uma folha, removemos ele.
        NO *lixo = *pt;
        (*pt) = NULL;
        free((*pt));
    }
    
}

int PreOrdem(NO *pt){
    if (pt != NULL){
        printf("\nvalor: %d, bal: %d\n", pt->valor, pt->bal);
        
        if (pt->esq != NULL){
            PreOrdem(pt->esq);
        }

        if (pt->dir != NULL){
            PreOrdem(pt->dir);
        }
    }
}