#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "consts.h"
#include "functions.c"

int main() 
{     
	if(SetConsoleOutputCP(1251)!=1) return -1;
	if(SetConsoleCP(1251)!=1) return -1;

	char field[SF][SF];
	char field_bot[SF][SF];
	char coords[5]={0};
	int res=0;
	int x, y;
	char buf;
	int offset_x=0, offset_y=0;
	int start_x=0,start_y=0;
	int start_x_user=0,start_y_user=0;
	char ort=0;
	int ship_count=10, ship_count_bot=10;
	char first_hit=0, hit_bot=0;
	char id=0;
	srand(time(NULL));
	
	for(int i=0; i<SF; i++)
	{
		for(int j=0; j<SF; j++)
		{
			field[i][j]='_';
			field_bot[i][j]='_';
		}
	}
	
	if(say_hello()!=0) return 1;

	if(create_field_auto(&field_bot)!=0) return 2;
	
	if(create_field(&field)!=0) return 3;
	
	if(draw_interface(field, field_bot, ship_count, ship_count_bot)!=0)
		{ return 4;}
	
	if(printf("Да начнется битва!\n")<0) return 5;
	
	while((ship_count_bot!=0) && (ship_count!=0))
	{
		if(id==USER_ID)
		{
			if(printf("Ведите координаты цели: ")<0) return 6;
			
			for(int i=0;i<5;i++){
				buf=getchar();
				if(buf!=0){
					if(buf!='\n'){
						coords[i]=buf;	
					}else{
						coords[i]=0; break;
					}
				}else{
					return 7;
				}
			}
			
			if(coords[4]!=0){
				while((buf=getchar())!='\n'){
					if(buf==0){
						return 8;
					}
				}
				coords[4]=0;
			}
			
			if(convert_coords(coords,&x,&y)!=0) return 9;
			
			if(check_coords(x, y, &res)!=0) return 10;
			
			if(res!=0){
				if(printf("Ошибка! Введены неверные координаты!\n") < 0) return 11;
				if(system("pause")==-1) return 12;
				if(system("cls")==-1) return 13;
				
				if(draw_interface(field, field_bot, ship_count, ship_count_bot)!=0) return 14;
				continue;
			}
			
			coords[2]=0;
			
			if((field_bot[y][x]=='o') || (field_bot[y][x]=='*'))
			{
				first_hit=0;
				if(printf("В эту клетку вы уже стреляли. Введите другую координату.\n")<0) return 15;
				continue;
			}
			
			field_bot[y][x]=(field_bot[y][x]=='@' ? '*' : 'o');
			
			if(draw_interface(field, field_bot, ship_count, ship_count_bot)!=0) return 16;
				
			if(field_bot[y][x]=='*')
			{
				if(first_hit==0){
					first_hit=1;
					start_y_user=y;
					start_x_user=x;				
				}
						
				if(printf("%s: Вы попали!\n", coords) < 0) return 17;
						
				if(check_ship(field_bot, y, x, &res)!=0) return 18;
				
				if(res==0){
					if((start_y_user!=y) || (start_x_user!=x)){
						if(check_ship(field_bot, start_y_user, start_x_user, &res)!=0) return 19;
					}
					
					if(res==0){
						start_x_user=0;
						start_y_user=0;
						first_hit=0;
						ship_count_bot--;
						
						if(draw_interface(field, field_bot, ship_count, ship_count_bot)!=0) return 20;
						
						if(printf("%s: Вы попали!\nКорабль уничтожен!\n", coords) < 0) return 21;
					}
				}
			}
			else
			{
				id=BOT_ID;
				
				if(printf("%s: Вы промахнулись!\n", coords) < 0) return 22;
			}
			
		}
		else
		{
			if((hit_bot!=0) || (ort!=0))
			{
				if((hit_bot!=0) && (ort!=0))
				{
					if(offset_x!=0)
					{
						x=start_x+(offset_x>0 ? ++offset_x : --offset_x);
					}
					else
					{
						y=start_y+(offset_y>0 ? ++offset_y : --offset_y);
					}
					
				}
				else 
				{
					if((offset_x>1) || (offset_x<-1) || (offset_y>1) || (offset_y<-1)) 
					{
						x=start_x;
						y=start_y;
						if(offset_x!=0){
							offset_x=(offset_x>0 ? -1 : 1);
						}
						else{
							offset_y=(offset_y>0 ? -1 : 1);
						}
						
					}
					else
					{
						offset_x=0;
						offset_y=0;
						
						switch(rand()%4)
						{
							case 0 :
								if((ort&LEFT)==0)
								{
									ort|=LEFT;
									if(start_x==0)	continue;
									
									offset_x--;
								}
								else continue;
								
								break;
							case 1 :
								if((ort&UP)==0){
									ort|=UP;	
									if(start_y==0) continue;
									
									offset_y--;
								}
								else continue;
												
								break;
							case 2 :
								if((ort&RIGHT)==0){
									ort|=RIGHT;
									if(start_x==SF-1) continue;
									
									offset_x++;
								}
								else continue;
								
								break;
							case 3 :
								if((ort&DOWN)==0){
									ort|=DOWN;
									if(start_y==SF-1) continue;
									
									offset_y++;
								}
								else continue;
							
								break;
						}
										
					}
					
					x=start_x+offset_x;
					y=start_y+offset_y;	
				}
									
			}
			else
			{
				x=rand()%SF;
				y=rand()%SF;
			}
			
			if((field[y][x]=='o') || (field[y][x]=='*') || (field[y][x]=='%'))
			{
				hit_bot=0;
				continue;
			}
			
			if(printf("Ход противника! Приготовьтесь!\n") < 0) return 23;
			
			sleep(2);
			field[y][x]=(field[y][x]=='@' ? '*' : 'o');
			
			if(draw_interface(field, field_bot, ship_count, ship_count_bot)!=0) return 24;
			
			if (y<9){
				if(printf("%c%c", x+'a', y+'1') < 0) return 25;
			}else{
				if(printf("%c%c%c", x+'a', (y/9)+'0', (y%9)+'0') < 0) return 26;
			}
					
			if(field[y][x]=='*')
			{
				if(ort==0)
				{
					start_x=x;
					start_y=y;
				}
				
				if(printf(": Противник попал!\n") < 0) return 27;
				
				hit_bot=1;
				check_ship(field, y, x, &res);
				
				if(res==0){
					if((x!=start_x) || (y!=start_y)){
						if(check_ship(field, start_y, start_x, &res)!=0) return 28;
					}
					
					if(res==0){
						if(block_cells(field, start_y, start_x)!=0) return 29;
						
						if(offset_x!=0){
							if(check_horizontal(field, start_y, start_x)!=0) return 30;	
							
						}else if(offset_y!=0){
							if(check_vertical(field, start_y, start_x)!=0) return 31;
						}
						
						offset_y=0;
						offset_x=0;
						start_x=0;
						start_y=0;
						ort=0;
						hit_bot=0;
						ship_count--;
						
						if(draw_interface(field, field_bot, ship_count, ship_count_bot)!=0) return 32;
					
						if (y<9){
							if(printf("%c%c", x+'a', y+'1') < 0) return 33;
						}else{
							if(printf("%c%c%c", x+'a', (y/9)+'0', (y%9)+'0') < 0) return 34;
						}
						
						if(printf(": Противник попал!\nКорабль уничтожен!\n") < 0) return 35;
						
						if(system("pause") == -1) return 36;
					}
				}
			}
			else 
			{
				hit_bot=0;
				id=USER_ID;
				
				if(printf(": Противник промахнулся\n") < 0) return 37;
			}
		}
	}
	
	if(ship_count==0){
		if(printf("Вы проиграли!\nНу что, еще разок?\n") < 0) return 38;
	}else{
		if(printf("Вы победили! Поздравляю!\n") < 0) return 39;
	}

	getchar();
	
	return 0;
 }
