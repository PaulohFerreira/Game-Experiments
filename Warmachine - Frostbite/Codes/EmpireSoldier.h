//============================================//
//            EMPIRE SOLDIER OF KOTHG
//============================================//
// Definições básicas
#define PI 3.14159265

#define FACE_EMPIRESOLDIER 7

//============================================//
//             VARIÁVEIS GLOBAIS
//============================================//
// Variáveis que operam no soldado como um todo
double pX_EmpireSoldier = 0.0, pY_EmpireSoldier = -0.5, pZ_EmpireSoldier = 0.0; // Posição e translação
double rX_EmpireSoldier = 0.0, rY_EmpireSoldier = 0.0, rZ_EmpireSoldier = 0.0; // Rotação
double sX_EmpireSoldier = 0.4, sY_EmpireSoldier = 0.4, sZ_EmpireSoldier = 0.4; // Escala
double animVelocity_EmpireSoldier = 0.03; // Velocidade de animação

// Variáveis que operam em partes específicas do soldado
int stateArmsPendulum_EmpireSoldier = 0; // 0 = Braço direito à frente; 1 = Braço esquerdo à frente; -1 = Nada
double rLeftArm_EmpireSoldier = 0.0;
double rRightArm_EmpireSoldier = 0.0;

int stateLegsPendulum_EmpireSoldier = 1; // 0 = Perna direita à frente; 1 = Perna esquerda à frente; -1 = Nada
double rLeftLeg_EmpireSoldier = 0.0;
double rRightLeg_EmpireSoldier = 0.0;

int colided_EmpireSoldier = 0; // 0 = Não colidiu com nada; 1 = Colidiu com algo;
int canDraw_EmpireSoldier = 0; // 0 = Não desenha o soldado; 1 = Desenha o soldado

int quantDefeated_EmpireSoldier = 0; // Guarda a quantidade de vezes


//============================================//
//       ANIMAÇÃO E MOVIMENTAÇÃO DO SOLDADO
//============================================//
void animateArms_EmpireSoldier(){ // Animação dos braços do soldado
    if(stateArmsPendulum_EmpireSoldier == 0){
        if(rLeftArm_EmpireSoldier < 45){
            rLeftArm_EmpireSoldier = rLeftArm_EmpireSoldier + animVelocity_EmpireSoldier;
            rRightArm_EmpireSoldier = rRightArm_EmpireSoldier - animVelocity_EmpireSoldier;
        }
        else stateArmsPendulum_EmpireSoldier = 1;
    }
    else if(stateArmsPendulum_EmpireSoldier == 1){
        if(rLeftArm_EmpireSoldier > -45){
            rLeftArm_EmpireSoldier = rLeftArm_EmpireSoldier - animVelocity_EmpireSoldier;
            rRightArm_EmpireSoldier = rRightArm_EmpireSoldier + animVelocity_EmpireSoldier;
        }
        else stateArmsPendulum_EmpireSoldier = 0;
    }
    glutPostRedisplay();
}
void animateLegs_EmpireSoldier(){ // Animação das pernas do soldado
    if(stateLegsPendulum_EmpireSoldier == 0){
        if(rLeftLeg_EmpireSoldier < 45){
            rLeftLeg_EmpireSoldier = rLeftLeg_EmpireSoldier + animVelocity_EmpireSoldier;
            rRightLeg_EmpireSoldier = rRightLeg_EmpireSoldier - animVelocity_EmpireSoldier;
        }
        else stateLegsPendulum_EmpireSoldier = 1;
    }
    else if(stateLegsPendulum_EmpireSoldier == 1){
        if(rLeftLeg_EmpireSoldier > -45){
            rLeftLeg_EmpireSoldier = rLeftLeg_EmpireSoldier - animVelocity_EmpireSoldier;
            rRightLeg_EmpireSoldier = rRightLeg_EmpireSoldier + animVelocity_EmpireSoldier;
        }
        else stateLegsPendulum_EmpireSoldier = 0;
    }
    glutPostRedisplay();
}
// Animação do soldado
void animate_EmpireSoldier(){
    animateArms_EmpireSoldier();
    animateLegs_EmpireSoldier();
}


//============================================//
//             TEXTURA DO SOLDADO
//============================================//
void loadTextures_EmpireSoldier(){
    CarregaTextura("face_EmpireSoldier.bmp", FACE_EMPIRESOLDIER);
}

//============================================//
//             MODELAGEM DO SOLDADO
//============================================//
void body_EmpireSoldier(){ // Cabeça e corpo do soldado
    // Cabeça
    glColor3d(1.0, 0.91, 0.73);
    glPushMatrix();
        glTranslated(0.0, -0.03, 0.0);

        // Quad para textura
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId_WarmachineKiwi[FACE_EMPIRESOLDIER]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.125f, 0.125f, -0.13f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.125f, 0.125f, -0.13f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.125f, -0.125f, -0.13f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.125f, -0.125f, -0.13f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        glutSolidCube(0.25);
    glPopMatrix();

    // Corpo
    glColor3d(0.8, 0.2, 0.2);
    glPushMatrix();
        glTranslated(0.0, -0.36, 0.0);
        glScaled(0.8, 1.4, 0.7);

        glutSolidCube(0.3);
    glPopMatrix();
}
void leftArm_EmpireSoldier(){ // Braço esquerdo do soldado
    glColor3d(0.8, 0.2, 0.2);
    glPushMatrix();
        glTranslated(0.165, -0.34, 0.0);

        // Arruma a posição
        glTranslated(0, 0.18, 0);
        // Rotação do braço
        glRotated(rLeftArm_EmpireSoldier, 1.0, 0.0, 0.0);
        // Deslocamento para o giro ficar certo
        glTranslated(0, -0.18, 0);

        glScaled(0.3, 1.2, 0.5);

        glutSolidCube(0.3);
    glPopMatrix();
}
void rightArm_EmpireSoldier(){ // Braço direito do soldado
    glColor3d(0.8, 0.2, 0.2);
    glPushMatrix();
        glTranslated(-0.165, -0.34, 0.0);

        // Arruma a posição
        glTranslated(0, 0.18, 0);
        // Rotação do braço
        glRotated(rRightArm_EmpireSoldier, 1.0, 0.0, 0.0);
        // Deslocamento para o giro ficar certo
        glTranslated(0, -0.18, 0);

        glScaled(0.3, 1.2, 0.5);

        glutSolidCube(0.3);
    glPopMatrix();
}
void leftLeg_EmpireSoldier(){ // Perna esquerda do soldado
    glColor3d(0.8, 0.2, 0.2);
    glPushMatrix();
        glTranslated(0.0755, -0.75, 0.0);

        // Arruma a posição
        glTranslated(0, 0.18, 0);
        // Rotação do braço
        glRotated(rLeftLeg_EmpireSoldier, 1.0, 0.0, 0.0);
        // Deslocamento para o giro ficar certo
        glTranslated(0, -0.18, 0);

        glScaled(0.3, 1.2, 0.5);

        glutSolidCube(0.3);
    glPopMatrix();
}
void rightLeg_EmpireSoldier(){ // Perna direita do soldado
    glColor3d(0.8, 0.2, 0.2);
    glPushMatrix();
        glTranslated(-0.0755, -0.75, 0.0);

        // Arruma a posição
        glTranslated(0, 0.18, 0);
        // Rotação do braço
        glRotated(rRightLeg_EmpireSoldier, 1.0, 0.0, 0.0);
        // Deslocamento para o giro ficar certo
        glTranslated(0, -0.18, 0);

        glScaled(0.3, 1.2, 0.5);

        glutSolidCube(0.3);
    glPopMatrix();
}

// Desenha todas as partes do soldado
void draw_EmpireSoldier(){
    // Teste de colisão
    // Se entra aqui, é porque ocorreu colisão
    if(!((pX_EmpireSoldier-pX_WarmachineKiwi)*(pX_EmpireSoldier-pX_WarmachineKiwi) > 0.1) && !((pZ_EmpireSoldier-pZ_WarmachineKiwi)*(pZ_EmpireSoldier-pZ_WarmachineKiwi) > 0.1)) colided_EmpireSoldier = 1;
    if(!((pX_EmpireSoldier-pX_WarmachineBLD)*(pX_EmpireSoldier-pX_WarmachineBLD) > 0.1) && !((pZ_EmpireSoldier-pZ_WarmachineBLD)*(pZ_EmpireSoldier-pZ_WarmachineBLD) > 0.1)) colided_EmpireSoldier = 1;


    // Quando colidir, gera outro inimigo em outro lugar
    if (colided_EmpireSoldier == 1){
        quantDefeated_EmpireSoldier++;
        pX_EmpireSoldier = -30 + rand()%60;
        pZ_EmpireSoldier = -30 + rand()%60;
        colided_EmpireSoldier = 0;
    }
    else canDraw_EmpireSoldier = 1;

    if(canDraw_EmpireSoldier == 1){
        // Movimentação do soldado
        glTranslated(pX_EmpireSoldier, pY_EmpireSoldier, pZ_EmpireSoldier);
        // Rotação do robô nos seus eixos
        glRotated(rX_EmpireSoldier, 1.0, 0.0, 0.0);
        glRotated(rY_EmpireSoldier, 0.0 ,1.0, 0.0);
        glRotated(rZ_EmpireSoldier, 0.0, 0.0, 1.0);
        // Escala do robô
        glScaled(sX_EmpireSoldier, sY_EmpireSoldier, sZ_EmpireSoldier);

        // Desenha as partes do soldado
        glPushMatrix();
            body_EmpireSoldier();
        glPopMatrix();

        glPushMatrix();
            leftArm_EmpireSoldier();
        glPopMatrix();

        glPushMatrix();
            rightArm_EmpireSoldier();
        glPopMatrix();

        glPushMatrix();
            leftLeg_EmpireSoldier();
        glPopMatrix();

        glPushMatrix();
            rightLeg_EmpireSoldier();
        glPopMatrix();

        glColor3d(1.0, 1.0, 1.0);
    }
}

