#include<bits/stdc++.h>
#include <easyx.h>
#pragma once
using namespace std;

/* graphics.h
https://blog.csdn.net/m0_74835231/article/details/142927676
cppgraphics.cpp 20250210 by hty*/
void cls(){
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(0,0,800,600);
}
IMAGE npi;
void initcppg(){
	loadimage(&npi,"lib\\nan.png",32,32);
	cout<<"cppg init ok\n";
}
void putimage1(int x,int y,IMAGE *img){
	if(img->getwidth()==0){
		putimage(x,y,&npi);
		return ;
	}
	putimage(x,y,img);
}
void transparentimage(int x,int y,IMAGE img){
	IMAGE img1;
	DWORD *d1;
	img1=img;
	if(img1.getwidth()==0){
		return ;
	}
	d1=GetImageBuffer(&img1);
	float h,s,l;
	for(int i=0;i<img1.getheight()*img1.getwidth();i++){
		RGBtoHSL(BGR(d1[i]),&h,&s,&l);
		if(l<0.03){
			d1[i]=BGR(WHITE);
		}
		if(d1[i]!=BGR(WHITE)){
			d1[i]=0;
		}
	}
	putimage(x,y,&img1,SRCAND);
	putimage(x,y,&img,SRCPAINT);
}
