//============================================//
//        	WARMACHINE "BLD"
//============================================//
// DefiniÁıes b·sicas
#define PI 3.14159265
#define NUMBER_OF_TEXTURES_WARMACHHINEKIWI 6

// CÛdigos das texturas
// Perna
#define FOOT_WARMACHINEKIWI 0
#define KNEE_WARMACHINEKIWI 1

// Corpo
#define FACE_WARMACHINEKIWI 2
#define EMBLEM_WARMACHINEKIWI 3

// Arma
#define GUNFRONT_WARMACHINEKIWI 4
#define GUNFACE_WARMACHINEKIWI 5

//============================================//
//             VARI¡VEIS GLOBAIS
//============================================//
// Vari·veis de movimentaÁ„o da c‚mera
double panX_WarmachineBLD = 0;
double panY_WarmachineBLD = 0;

// Vari·veis que operam no robÙ como um todo
double pX_WarmachineBLD = 0.0, pY_WarmachineBLD = 0.0, pZ_WarmachineBLD = 0.0; // PosiÁ„o e translaÁ„o
double rX_WarmachineBLD = 0.0, rY_WarmachineBLD = 0.0, rZ_WarmachineBLD = 0.0; // RotaÁ„o
double sX_WarmachineBLD = 1.0, sY_WarmachineBLD = 1.0, sZ_WarmachineBLD = 1.0; // Escala
double animVelocity_WarmachineBLD = 2.58; // Velocidade de animaÁ„o

// Vari·veis que operam em partes especÌficas do robÙ
// Perna Esquerda
int stateLeftLegPendulum_WarmachineBLD = 1; // 0 = Perna balanÁa para frente; 1 = Perna balanÁa para tr·s; -1 = fica parado
double rLeftLegFirst_WarmachineBLD = 0.0; // Primeiro parte mÛvel de cima para baixo
double rLeftLegSecond_WarmachineBLD = 0.0; // Segundo parte mÛvel de cima para baixo
double rLeftLegThird_WarmachineBLD = 0.0; // Terceiro parte mÛvel de cima para baixo
double rLeftLegFourth_WarmachineBLD = 0.0; // Quarta parte mÛvel de cima para baixo (pÈ)

// Perna Direita
int stateRightLegPendulum_WarmachineBLD = 0; // 0 = Perna balanÁa para frente; 1 = Perna balanÁa para tr·s; -1 = fica parado
double rRightLegFirst_WarmachineBLD = 0.0; // Primeiro parte mÛvel de cima para baixo
double rRightLegSecond_WarmachineBLD = 0.0; // Segundo parte mÛvel de cima para baixo
double rRightLegThird_WarmachineBLD = 0.0; // Terceiro parte mÛvel de cima para baixo
double rRightLegFourth_WarmachineBLD = 0.0; // Quarta parte mÛvel de cima para baixo (pÈ)


//============================================//
//              C¬MERA DO ROB‘
//============================================//

void isometricCamera_WarmachineBLD(float fAspect){ // C‚mera isomÈtrica - VisualizaÁ„o t·tica
    // Troca o modo da matriz para ajustar a c‚mera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Define a c‚mera como ortogonal
    glOrtho(-10.0f*fAspect - panX_WarmachineBLD, 10.0f*fAspect - panX_WarmachineBLD, -10.0f*fAspect - panY_WarmachineBLD, 10.0f*fAspect - panY_WarmachineBLD, -60.0f*fAspect, 60.0f*fAspect);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Rotaciona nos eixos para que a angulaÁ„o isomÈtrica fique correta
    glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
}
void perspectiveCamera_WarmachineBLD(float fAspect){ // C‚mera perspectiva - VisualizaÁ„o de comando
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Cria a c‚mera em perspectiva
    gluPerspective(60.0, fAspect, 0.1, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Posiciona a c‚mera acima no robÙ
    gluLookAt(pX_WarmachineBLD + 2*cos((PI/180)*(rY_WarmachineBLD - 90)),
              pY_WarmachineBLD + 1.25,
              pZ_WarmachineBLD - 2*sin((PI/180)*(rY_WarmachineBLD - 90)),

              pX_WarmachineBLD,
              pY_WarmachineBLD,
              pZ_WarmachineBLD,

              0,1,0);
}


//============================================//
//              ANIMA«√O DO ROB‘
//============================================//
void resetFlags_WarmachineBLD(){ // Reseta as flags de animaÁ„o
    stateLeftLegPendulum_WarmachineBLD = 1; // 0 = Perna balanÁa para frente; 1 = Perna balanÁa para tr·s; -1 = fica parado
    rLeftLegFirst_WarmachineBLD = 0.0; // Primeiro parte mÛvel de cima para baixo
    rLeftLegSecond_WarmachineBLD = 0.0; // Segundo parte mÛvel de cima para baixo
    rLeftLegThird_WarmachineBLD = 0.0; // Terceiro parte mÛvel de cima para baixo
    rLeftLegFourth_WarmachineBLD = 0.0; // Quarta parte mÛvel de cima para baixo (pÈ)

    // Perna Direita
    stateRightLegPendulum_WarmachineBLD = 0; // 0 = Perna balanÁa para frente; 1 = Perna balanÁa para tr·s; -1 = fica parado
    rRightLegFirst_WarmachineBLD = 0.0; // Primeiro parte mÛvel de cima para baixo
    rRightLegSecond_WarmachineBLD = 0.0; // Segundo parte mÛvel de cima para baixo
    rRightLegThird_WarmachineBLD = 0.0; // Terceiro parte mÛvel de cima para baixo
    rRightLegFourth_WarmachineBLD = 0.0; // Quarta parte mÛvel de cima para baixo (pÈ)
}
void walkLeftLeg_WarmachineBLD(){// Anima a perna esquerda
    if(stateLeftLegPendulum_WarmachineBLD == 0){
        // Primeiro pedaÁo da perna
        if(rLeftLegFirst_WarmachineBLD < 30){
            rLeftLegFirst_WarmachineBLD = rLeftLegFirst_WarmachineBLD + animVelocity_WarmachineBLD;

            // Segundo pedaÁo da perna
            if(rLeftLegFirst_WarmachineBLD >= 0) rLeftLegSecond_WarmachineBLD = rLeftLegSecond_WarmachineBLD + animVelocity_WarmachineBLD;
            else rLeftLegSecond_WarmachineBLD = rLeftLegSecond_WarmachineBLD - animVelocity_WarmachineBLD;

            // Terceiro pedaÁo da perna
            if(rLeftLegFirst_WarmachineBLD >= 0) rLeftLegThird_WarmachineBLD = rLeftLegThird_WarmachineBLD - animVelocity_WarmachineBLD;
            else rLeftLegThird_WarmachineBLD = rLeftLegThird_WarmachineBLD + animVelocity_WarmachineBLD;

            // Quarto pedaÁo da perna (pÈ)
            rLeftLegFourth_WarmachineBLD = rLeftLegFourth_WarmachineBLD - animVelocity_WarmachineBLD;
        }

        else stateLeftLegPendulum_WarmachineBLD = 1;
    }
    else if(stateLeftLegPendulum_WarmachineBLD == 1){
        if(rLeftLegFirst_WarmachineBLD > -30){
            rLeftLegFirst_WarmachineBLD = rLeftLegFirst_WarmachineBLD - animVelocity_WarmachineBLD;

            // Segundo pedaÁo da perna
            if(rLeftLegFirst_WarmachineBLD >= 0) rLeftLegSecond_WarmachineBLD = rLeftLegSecond_WarmachineBLD - animVelocity_WarmachineBLD;
            else rLeftLegSecond_WarmachineBLD = rLeftLegSecond_WarmachineBLD + animVelocity_WarmachineBLD;

            // Terceiro pedaÁo da perna
            if(rLeftLegFirst_WarmachineBLD >= 0) rLeftLegThird_WarmachineBLD = rLeftLegThird_WarmachineBLD + animVelocity_WarmachineBLD;
            else rLeftLegThird_WarmachineBLD = rLeftLegThird_WarmachineBLD - animVelocity_WarmachineBLD;

            // Quarto pedaÁo da perna (pÈ)
            rLeftLegFourth_WarmachineBLD = rLeftLegFourth_WarmachineBLD + animVelocity_WarmachineBLD;
        }
        else stateLeftLegPendulum_WarmachineBLD = 0;
    }

    glutPostRedisplay();
}
void walkRightLeg_WarmachineBLD(){// Anima a perna direita
    if(stateRightLegPendulum_WarmachineBLD == 0){
        // Primeiro pedaÁo da perna
        if(rRightLegFirst_WarmachineBLD < 30){
            rRightLegFirst_WarmachineBLD = rRightLegFirst_WarmachineBLD + animVelocity_WarmachineBLD;

            // Segundo pedaÁo da perna
            if(rRightLegFirst_WarmachineBLD >= 0) rRightLegSecond_WarmachineBLD = rRightLegSecond_WarmachineBLD + animVelocity_WarmachineBLD;
            else rRightLegSecond_WarmachineBLD = rRightLegSecond_WarmachineBLD - animVelocity_WarmachineBLD;

            // Terceiro pedaÁo da perna
            if(rRightLegFirst_WarmachineBLD >= 0) rRightLegThird_WarmachineBLD = rRightLegThird_WarmachineBLD - animVelocity_WarmachineBLD;
            else rRightLegThird_WarmachineBLD = rRightLegThird_WarmachineBLD + animVelocity_WarmachineBLD;

            // Quarto pedaÁo da perna (pÈ)
            rRightLegFourth_WarmachineBLD = rRightLegFourth_WarmachineBLD - animVelocity_WarmachineBLD;
        }

        else stateRightLegPendulum_WarmachineBLD = 1;
    }
    else if(stateRightLegPendulum_WarmachineBLD == 1){
        if(rRightLegFirst_WarmachineBLD > -30){
            rRightLegFirst_WarmachineBLD = rRightLegFirst_WarmachineBLD - animVelocity_WarmachineBLD;

            // Segundo pedaÁo da perna
            if(rRightLegFirst_WarmachineBLD >= 0) rRightLegSecond_WarmachineBLD = rRightLegSecond_WarmachineBLD - animVelocity_WarmachineBLD;
            else rRightLegSecond_WarmachineBLD = rRightLegSecond_WarmachineBLD + animVelocity_WarmachineBLD;

            // Terceiro pedaÁo da perna
            if(rRightLegFirst_WarmachineBLD >= 0) rRightLegThird_WarmachineBLD = rRightLegThird_WarmachineBLD + animVelocity_WarmachineBLD;
            else rRightLegThird_WarmachineBLD = rRightLegThird_WarmachineBLD - animVelocity_WarmachineBLD;

            // Quarto pedaÁo da perna (pÈ)
            rRightLegFourth_WarmachineBLD = rRightLegFourth_WarmachineBLD + animVelocity_WarmachineBLD;
        }
        else stateRightLegPendulum_WarmachineBLD = 0;
    }

    glutPostRedisplay();
}
// Anima as duas pernas
void walk_WarmachineBLD(){
    walkLeftLeg_WarmachineBLD();
    walkRightLeg_WarmachineBLD();
}


//============================================//
//              TEXTURAS DO ROB‘
//============================================//
/*void loadTextures_WarmachineKiwi(){

    // Perna
    CarregaTextura("foot_WarmachineKiwi.bmp", FOOT_WARMACHINEKIWI);

    // Corpo
    CarregaTextura("face_WarmachineKiwi.bmp", FACE_WARMACHINEKIWI);
    CarregaTextura("emblem_WarmachineKiwi.bmp", EMBLEM_WARMACHINEKIWI);

    // Arma
    CarregaTextura("gunFront_WarmachineKiwi.bmp", GUNFRONT_WARMACHINEKIWI);
    CarregaTextura("gunFace_WarmachineKiwi.bmp", GUNFACE_WARMACHINEKIWI);
}*/


//============================================//
//              MODELAGEM DO ROB‘
//============================================//
void base_warMachineBLD(){

    //corpo
    glPushMatrix();
        glColor3f(1.0, 0.843, 0.0);
        glTranslated(0.0,0.35,0.0);
        glScaled(0.85, 0.85, 1.2);
        glutSolidCube(0.5);
    glPopMatrix();
    //conexão do corpo com a cintura
    glPushMatrix();
        glColor3f(0.502, 0.502, 0.502);
        glTranslated(0.0,0.1,0.0);
        glScaled(0.2, 0.2, 1.0);
        //glTranslated(0.0, -1.0, 0.0);
        glutSolidCube(0.5);
    glPopMatrix();
    //cintura/ liga o corpo com as pernas
    glPushMatrix();
        glColor3f(1.0, 0.843, 0.0);
        glScaled(0.65, 0.2, 1.0);
        //glTranslated(0.0, -1.5, 0.0);
        glutSolidCube(0.5);
    glPopMatrix();
    //suporte de arma lado direito
    glPushMatrix();
        glColor3f(0.502, 0.502, 0.502);
        glTranslated(0.27,0.4, -0.15);
        glScaled(0.3, 0.1, 0.1);
        glutSolidCube(0.5);
    glPopMatrix();
    //suporte de arma 1 lado esquerdo
    glPushMatrix();
        glColor3f(0.502, 0.502, 0.502);
        glTranslated(-0.27,0.25, -0.15);
        glScaled(0.3, 0.1, 0.1);
        glutSolidCube(0.5);
    glPopMatrix();
    //suporte de arma 1 lado esquerdo
    glPushMatrix();
        glColor3f(0.502, 0.502, 0.502);
        glTranslated(-0.27,0.45, -0.15);
        glScaled(0.3, 0.1, 0.1);
        glutSolidCube(0.5);
    glPopMatrix();

}
void leftLeg_warMachineBLD(){
    glColor3d(0.15, 0.15, 0.15);

    glTranslated(-0.185, -0.0375, -0.05);
    // RotaÁ„o da perna toda
    glRotated(rLeftLegFirst_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o giro ficar certo
    glTranslated(0, -0.0625, 0);

    // parte superior

    glPushMatrix();
        glRotated(20.0, 1.0 ,0.0, 0.0);
        glScaled(0.1, 0.5, 0.25);
        glutSolidCube(0.5);
    glPopMatrix();

    // conexão entre a parte superior e inferior da perna
    glPushMatrix();
        glTranslated(-0.01, -0.1, -0.1);
        glScaled(0.75, 1.4, 1.0);
        glutSolidCube(0.15);
    glPopMatrix();

    // Arruma a posicao
    glTranslated(0, -0.0625, -0.085);
    // Rotacao das partes abaixo
    glRotated(rLeftLegSecond_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o giro ficar certo
    glTranslated(0, 0.0625, 0.085);

    // Segunda parte da perna
    glPushMatrix();
        glTranslated(0.0, -0.10, 0.07);
        glScaled(0.12, 0.25, 0.6);
        glutSolidCube(0.5);
    glPopMatrix();

    // Arruma a posicao
    glTranslated(0.0 ,-0.0875 , 0.18);
    // Rotacao das partes abaixo
    glRotated(rLeftLegThird_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o eixo de giro ficar certo
    glTranslated(0.0, -0.2915, -0.0725);
    glTranslated(0.0 ,0.32 , -0.15);

    // parte inferior da perna
    glPushMatrix();
        glTranslated(0.0 ,-0.32 , 0.15);
        glRotated(10.0, 1.0 ,0.0, 0.0);
        glScaled(0.12, 1.25, 0.25);
        glutSolidCube(0.5);
    glPopMatrix();


    // Arruma a posicao
    glTranslated(0.0 ,-0.62, 0.06);
    // Rotacao das partes da perna que estao abaixo
    glRotated(rLeftLegFourth_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o eixo de giro ficar certo
    glTranslated(0.0 ,0.62, -0.06);

    // PÈ
    glPushMatrix();
        glTranslated(0.0 ,-0.62, 0.06);
        glScaled(0.08, 0.08, 0.16);

        glEnable(GL_TEXTURE_2D);
        //glBindTexture (GL_TEXTURE_2D, textureId_WarmachineKiwi[FOOT_WARMACHINEKIWI]);
        glBegin(GL_TRIANGLES);
            // Face da frente
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

            // Face da direita
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

            // Face de tras
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

            // Face da esquerda
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
void rightLeg_warMachineBLD(){
    glColor3d(0.15, 0.15, 0.15);

    glTranslated(0.185, -0.0375, -0.05);
    // RotaÁ„o da perna toda
    glRotated(rRightLegFirst_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o giro ficar certo
    glTranslated(0, -0.0625, 0);

    // Primeira parte da perna
    glPushMatrix();
        glRotated(20.0, 1.0 ,0.0, 0.0);
        glScaled(0.1, 0.5, 0.25);
        glutSolidCube(0.5);
    glPopMatrix();

    // Junta da perna entre partes 1 e 2
    glPushMatrix();
        glTranslated(0.01, -0.1, -0.1);
        glScaled(0.75, 1.4, 1.0);
        glutSolidCube(0.15);
    glPopMatrix();

    // Arruma a posiÁ„o
    glTranslated(0, -0.0625, -0.085);
    // RotaÁ„o das partes da perna que est„o abaixo
    glRotated(rRightLegSecond_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o giro ficar certo
    glTranslated(0, 0.0625, 0.085);

    // Segunda parte da perna
    glPushMatrix();
        glTranslated(0.0, -0.10, 0.07);
        glScaled(0.12, 0.25, 0.6);
        glutSolidCube(0.5);
    glPopMatrix();

    // Arruma a posiÁ„o
    glTranslated(0.0 ,-0.0875 , 0.18);
    // RotaÁ„o das partes da perna que est„o abaixo
    glRotated(rRightLegThird_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o eixo de giro ficar certo
    glTranslated(0.0, -0.2915, -0.0725);
    glTranslated(0.0 ,0.32 , -0.15);

    // Terceira parte da perna
    glPushMatrix();
        glTranslated(0.0 ,-0.32 , 0.15);
        glRotated(10.0, 1.0 ,0.0, 0.0);
        glScaled(0.12, 1.25, 0.25);
        glutSolidCube(0.5);
    glPopMatrix();


    // Arruma a posiÁ„o
    glTranslated(0.0 ,-0.62, 0.06);
    // RotaÁ„o das partes da perna que est„o abaixo
    glRotated(rRightLegFourth_WarmachineBLD, 1.0, 0.0, 0.0);
    // Deslocamento para o eixo de giro ficar certo
    glTranslated(0.0 ,0.62, -0.06);

    // PÈ
    glPushMatrix();
        glTranslated(0.0 ,-0.62, 0.06);
        glScaled(0.08, 0.08, 0.16);

        glEnable(GL_TEXTURE_2D);
        //glBindTexture (GL_TEXTURE_2D, textureId_WarmachineKiwi[FOOT_WARMACHINEKIWI]);
        glBegin(GL_TRIANGLES);
        // Face da frente
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

            // Face da direita
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

            // Face de tr·s
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

            // Face da esquerda
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 1.0f, 0.0f);
            glTexCoord2f(-1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
void gun_warMachineBLD(){
    //suporte da arma lado direito
    glPushMatrix();
        glColor3f(1.0, 0.843, 0.0);
        glTranslated(0.42,0.45, -0.15);
        glScaled(0.35, 0.6, 0.4);
        glutSolidCube(0.5);
    glPopMatrix();
    //arma superior da direita
    glPushMatrix();
        glColor3f(0.502, 0.502, 0.502);
        glTranslated(0.42,0.55, -0.45);
        glScaled(0.12, 0.12, 1.0);
        glutSolidCube(0.5);
    glPopMatrix();
    //arma inferior da direita
    glPushMatrix();
        glColor3f(0.502, 0.502, 0.502);
        glTranslated(0.42,0.35, -0.45);
        glScaled(0.12, 0.12, 1.0);
        glutSolidCube(0.5);
    glPopMatrix();
    //suporte da arma do lado esquerdo(foguetes)
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(-0.43,0.35, -0.15);
        glScaled(0.4, 0.85, 0.4);
        glutSolidCube(0.5);
    glPopMatrix();
    //arma sobre a cabeça
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(0.0,0.61, 0.1);
        glScaled(0.65, 0.2, 0.7);
        glutSolidCube(0.5);
    glPopMatrix();
    //misseis
    glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslated(-0.47, 0.50, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.47, 0.40, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.47, 0.30, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.47, 0.20, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.37, 0.50, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.37, 0.40, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.37, 0.30, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.37, 0.20, -0.25);
        glutSolidSphere(0.035, 10, 3);
    glPopMatrix();
    //misseis superiores
    glPushMatrix();
        glTranslated(0.10, 0.61, -0.08);
        glutSolidSphere(0.03, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0.0, 0.61, -0.08);
        glutSolidSphere(0.03, 10, 3);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-0.10, 0.61, -0.08);
        glutSolidSphere(0.03, 10, 3);
    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
}
// Desenha todas as peÁas do robÙ juntas
void draw_WarmachineBLD(){

    // MovimentaÁ„o do robÙ
    glTranslated(pX_WarmachineBLD, pY_WarmachineBLD, pZ_WarmachineBLD);
    // RotaÁ„o do robÙ nos seus eixos
    glRotated(rX_WarmachineBLD, 1.0, 0.0, 0.0);
    glRotated(rY_WarmachineBLD, 0.0, 1.0, 0.0);
    glRotated(rZ_WarmachineBLD, 0.0, 0.0, 1.0);
    // Escala do robÙ
    glScaled(sX_WarmachineBLD, sY_WarmachineBLD, sZ_WarmachineBLD);

    // Desenha as partes do robÙ
    glPushMatrix();
    base_warMachineBLD();
    glPopMatrix();

    glPushMatrix();
    leftLeg_warMachineBLD();
    glPopMatrix();

    glPushMatrix();
    rightLeg_warMachineBLD();
    glPopMatrix();

    glPushMatrix();
    gun_warMachineBLD();
    glPopMatrix();
}

//============================================//
//           MOVIMENTA«√O DO ROB‘
//============================================//
void moveFoward_WarmachineBLD(){
    if(pZ_WarmachineBLD >= -50 && pZ_WarmachineBLD <= 50) pZ_WarmachineBLD -= 0.08*cos((PI/180)*rY_WarmachineBLD);
    else pZ_WarmachineBLD += 0.08*cos((PI/180)*rY_WarmachineBLD);
    if(pX_WarmachineBLD >= -50 && pX_WarmachineBLD <= 50) pX_WarmachineBLD -= 0.08*sin((PI/180)*rY_WarmachineBLD);
    else pX_WarmachineBLD += 0.08*sin((PI/180)*rY_WarmachineBLD);
}
void moveBackward_WarmachineBLD(){
    if(pZ_WarmachineBLD >= -50 && pZ_WarmachineBLD <= 50) pZ_WarmachineBLD += 0.08*cos((PI/180)*rY_WarmachineBLD);
    else pZ_WarmachineBLD -= 0.08*cos((PI/180)*rY_WarmachineBLD);
    if(pX_WarmachineBLD >= -50 && pX_WarmachineBLD <= 50) pX_WarmachineBLD += 0.08*sin((PI/180)*rY_WarmachineBLD);
    else pX_WarmachineBLD -= 0.08*sin((PI/180)*rY_WarmachineBLD);
}
void turnRight_WarmachineBLD(){
    rY_WarmachineBLD -= 1.5;
}
void turnLeft_WarmachineBLD(){
    rY_WarmachineBLD += 1.5;
}
