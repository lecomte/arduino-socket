#include <bits/stdc++.h>

using namespace std;

char jogador, robo='X', mat[3][3];

void makeMoveJogador(int x, int y){
    mat[x][y]=jogador;
}

void imprime(){
    cout << endl;
    cout << mat[0][0] << "|" << mat[0][1] << "|" << mat[0][2] << endl;
    cout << "-+-+-\n";
    cout << mat[1][0] << "|" << mat[1][1] << "|" << mat[1][2] << endl;
    cout << "-+-+-\n";
    cout << mat[2][0] << "|" << mat[2][1] << "|" << mat[2][2] << endl;
    cout << endl;
}

bool jogoTerminado(){
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            if (mat[i][j] == ' ') return false;
    return true;
}

int euristica(){
    for (int i=0; i<3; i++){
        if (mat[i][0] == mat[i][1] && mat[i][1] == mat[i][2]){
            if (mat[i][0] == jogador)
                return -10;
            else if (mat[i][1] == robo)
                return 10;
        }
        if (mat[0][i] == mat[1][i] && mat[1][i] == mat[2][i]){
            if (mat[0][i] == jogador)
                return -10;
            else if (mat[1][i] == robo)
                return 10;
        }
    }
    if (mat[0][0] == mat[1][1] && mat[1][1] == mat[2][2]){
        if (mat[1][1] == jogador)
            return -10;
        else if (mat[1][1] == robo)
            return 10;
    }
    if (mat[0][2] == mat[1][1] && mat[1][1] == mat[2][0]){
        if (mat[1][1] == jogador)
            return -10;
        else if (mat[1][1] == robo)
            return 10;
    }
    return 0;
}

int minimax(bool flag){
    int valor=euristica();
    if (valor == 10) return valor;
    if (valor == -10) return valor;
    if (jogoTerminado()) return 0;
    int melhor;
    if (flag){
        melhor = -1000000;
        for (int i=0; i<3; i++)
            for (int j=0; j<3; j++){
                if (mat[i][j] == ' '){
                    mat[i][j]=robo;
                    melhor=max(melhor, minimax(!flag));
                    mat[i][j]=' ';
                }
            }
    }
    else{
        melhor = 10000000;
        for (int i=0; i<3; i++)
            for (int j=0; j<3; j++){
                if (mat[i][j] == ' '){
                    mat[i][j]=jogador;
                    melhor=min(melhor, minimax(!flag));
                    mat[i][j]=' ';
                }
            }
    }
    return melhor;
}

void makeMoveRobo(){
    int melhorMovimento = -1000000, x, y, valorMovimento;
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            if (mat[i][j]==' '){
                mat[i][j]=robo;
                valorMovimento = minimax(false);
                mat[i][j]=' ';
                if (valorMovimento > melhorMovimento){
                    melhorMovimento = valorMovimento;
                    x=i; y=j;
                }
            }
        }
    }
    cout << x << " " << y << endl;
    mat[x][y]=robo;
}

int main(){
    cin >> jogador;
    if (jogador == 'X') robo='O';
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            mat[i][j]=' ';
    while(euristica()==0 && !jogoTerminado()){
        imprime();
        int x, y;
        cin >> x >> y;
        while(x<0 || x>2 || y<0 || y>2 || mat[x][y]!=' '){
            cout << "Jogada invalida, escolha outra jogada\n";
            cin >> x >> y;
        }
        makeMoveJogador(x, y);
        imprime();
        if (euristica()==0 && !jogoTerminado())
            makeMoveRobo();
    }
    imprime();
    cout << "Jogo acabou\n";
}