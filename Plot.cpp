#include "Plot.h"
using namespace std;

QPixmap* createPlot(FuncReturningValue* frv)
{
    QPixmap *pix = new QPixmap(PIX_MAP_H, PIX_MAP_W);
    QPainter paint(pix);
    paint.fillRect(0, 0, PIX_MAP_H, PIX_MAP_W, QBrush(QColor(Qt::GlobalColor::white)));
    if(frv->solution[MIN_VALUE_INDEX] != EMPTY){
        drawAxis(frv, &paint);
        drawPoints(frv, &paint);
    }
    return pix;
}

void drawAxis(FuncReturningValue *frv, QPainter* paint)
{
    paint->drawLine(S_X, S_Y, S_X, (S_Y - LEN_Y)); // Oy
    paint->drawLine(S_X, S_Y, (S_X + LEN_X), S_Y); // Ox
    QPen pen(QColor(QColor(50,50,50)));
    paint->setPen(pen);
    frv->horStep = LEN_X/frv->valuesXSize;
    frv->vertStep = LEN_Y/VALUES_Y_SIZE;
    for(int i = 0; i < VALUES_Y_SIZE; i++){
        paint->drawLine(S_X-MARGIN/2, S_Y-frv->vertStep*(i), S_X+MARGIN/2, S_Y-frv->vertStep*(i));
        paint->drawText(S_X-MARGIN*4, S_Y-(frv->vertStep*(i)+MARGIN), (QString::number(frv->valuesY[i])));
        frv->lenUsingY = frv->vertStep*i;
    }
    paint->drawLine(S_X, S_Y-(frv->vertStep*(VALUES_Y_SIZE-1)), S_X + LEN_X, S_Y - (frv->vertStep*(VALUES_Y_SIZE-1)));
    paint->drawText(S_X+MARGIN/2, S_Y-(frv->vertStep*VALUES_Y_SIZE), "MAX:" + QString::number(frv->solution[MAX_VALUE_INDEX]));
    int compensatorX = 0;
    for(int i = 0; i < frv->valuesXSize; i++){
        if(frv->plotValues[i] != EMPTY){
            paint->drawLine(S_X+frv->horStep*(i-compensatorX), S_Y-MARGIN/2, S_X+frv->horStep*(i-compensatorX), S_Y+MARGIN/2);
            paint->drawText(S_X+(frv->horStep*(i-compensatorX)-MARGIN), S_Y+MARGIN, (QString::number(frv->valuesX[i])));
        }else
            compensatorX++;
    }
    paint->drawText(PIX_MAP_H - MARGIN*5, S_Y+MARGIN,  QString("Years"));
    frv->plotStep = frv->lenUsingY/(frv->solution[MAX_VALUE_INDEX] - frv->solution[MIN_VALUE_INDEX]);
    paint->setPen(QColor(Qt::GlobalColor::red));
    paint->drawLine(S_X, S_Y - (frv->solution[MID_VALUE_INDEX] - frv->solution[MIN_VALUE_INDEX]) * frv->plotStep, (S_X + LEN_X), S_Y - (frv->solution[MID_VALUE_INDEX] - frv->solution[MIN_VALUE_INDEX]) * frv->plotStep);
    paint->drawText((S_X + LEN_X)-MARGIN*2, S_Y - (frv->solution[MID_VALUE_INDEX] - frv->solution[MIN_VALUE_INDEX]) * frv->plotStep - MARGIN, "MID:" + QString::number(frv->solution[MID_VALUE_INDEX]));
}

void drawPoints(FuncReturningValue *frv, QPainter* paint)
{
    int compensatorP = 0;
    QPoint point[frv->plotValuesSize];
    for(int i = 0; i < frv->plotValuesSize;){
        if(frv->plotValues[i] != EMPTY)
            point[i] = QPoint(S_X + (frv->horStep*(i-compensatorP)), S_Y - (frv->plotValues[i] - frv->solution[MIN_VALUE_INDEX]) * frv->plotStep);
        else
            compensatorP++;
        i++;
    }
    paint->setRenderHint(QPainter::Antialiasing);
    paint->setPen(Qt::NoPen);
    paint->setBrush(QColor(Qt::GlobalColor::blue));
    for(int i = 0; i < frv->valuesXSize; i++){
        paint->drawEllipse(point[i], 5, 5);
    }
}




//    paint->drawText(S_X-MARGIN*4, MARGIN,  QString("npg"));
