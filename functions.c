int say_hello()
{
	if(printf("Добро пожаловать в игру \"Морской бой\"!\n\nПравила игры\nРасстановка кораблей:\n\n") < 0) return 5;
	if(printf("Для установки корабля введите его координаты в форме: буква-число-буква\n") < 0) return 6;
	if(printf("Координата указывается для первой (носовой) клетки корабля\n") < 0) return 7;
	if(printf("Все остальные клетки поставятся взависимости от напрвления, которое вы укажете\n") < 0) return 8;
	if(printf("Где первая буква отвечает за кооринату по горизонтали. От a до %c\n", SF+'a'-1) < 0) return 9;
	if(printf("Число отвечает за координату по вертикали. От 1 до %d\n", SF) < 0) return 10;
	if(printf("Вторая буква отвечает за направление:\n\th - расположить корабль по горизонтали\n") < 0) return 11;
	if(printf("\tv - расположить корабль по вертикали\n") < 0) return 12;
	if(printf("Для однопалубного корабля напрвление можно не указывать\n\n") < 0) return 13;
	if(printf("Стрельба:\n\n") < 0) return 14;
	if(printf("Для произведения выстрела введите координату в том же виде, за исключением напрвления\n") < 0) return 15;
	if(printf("Если вы попали, это место на поле будет обозначено символом \"*\"\n") < 0) return 16;
	if(printf("В противном случаем символом \"o\"\n\n") < 0) return 17;
	if(printf("Если все понятно, нажмите любую клавишу\n") < 0) return 18;
	if(getch()==0) return 19;
	if(system("cls")==-1) return 20;
	
	return 0;
}

int draw(char p[SF][SF], char show)
{
	if(p==NULL) return 1;
	
	char c='a';
	if(printf("  ") < 0) return 5;

	for(c='a';c<('a'+SF);c++){
		if(printf(" %c",c) < 0) return 6;
	}
	if(printf("\n  ") < 0) return 7;
	
	for(int i=0;i<SF;i++){
		if(printf(" _") < 0) return 8;
	}
	if(printf("\n") < 0) return 9;
	
	for(int i=0;i<SF;i++){
		if(printf("%d",i+1) < 0) return 10;
		
		if(i<9){
			if(printf(" ") < 0) return 11;
		}	
					
		for(int j=0;j<SF;j++){
			if(p[i][j]!='%'){
				if(p[i][j]!='@'){
					if(printf("|%c", p[i][j]) < 0) return 12;
				}else{
					if(show==1){if(printf("|@")<0) return 13;}
					else if(printf("|_") < 0) return 14;
				}
			}else
				if(printf("|_") < 0) return 15;
		}
			if(printf("|\n") < 0) return 16;
	}
	if(printf("\n") < 0) return 17;
	return 0;
}

int draw_interface(char field[SF][SF], char field_bot[SF][SF], int ship_count, int ship_count_bot)
{
	if(field==NULL){ return 1;}
	if(field_bot==NULL){ return 2;}

	if(system("cls")==-1) return 5;
	if(printf("\t\t\t\t\tКорабли пользователя: %d\n", ship_count) < 0) return 6;
	if(printf("\t\t\t\t\tКорабли противника:   %d\n", ship_count_bot) < 0) return 7;
	if(printf("   Поле противника: \n\n") < 0) return 8;
	if(draw(field_bot, NOT_SHOW)!=0) return 9;
	if(printf("   Поле пользователя:\n\n") < 0) return 10;
	if(draw(field, SHOW)!=0) return 11;
	
	return 0;
}


int generate_coords(char (*coords)[5])
{
	if(coords==NULL) return 1;
	
	char rand_num;
		
	(*coords)[0]=rand()%SF+1+'a';
	
	rand_num=rand()%SF+1;
	
	if(rand_num<10)
	{
		(*coords)[1]=rand_num;
	}
	else
	{
		(*coords)[1]=rand_num/10;
		(*coords)[2]=rand_num%10+'0';
	}
	(*coords)[1]+='0';
			
	if(rand_num<10)
	{
		(*coords)[2]=(rand()%2==0 ? 'h': 'v');
	}
	else
	{
		(*coords)[3]=(rand()%2==0 ? 'h': 'v');
	}
	
	return 0;
}

int convert_coords(char str[5], int* x, int* y )
{
	if(str==NULL) return 1;
	if(x==NULL) return 2;
	if(y==NULL) return 3;
	
	*x=(int) (str[0]-'a');
	
	if((str[2]<'a')&&(str[2]!=0) )
	{
		*y=(str[1]-'0')*10+(str[2]-'0')-1;
	}
	else
	{
		*y=(int) (str[1] - '0')-1;
	}
	return 0;
}

int check_coords(int x, int y, int* res){
	if(res==NULL) return 3;
	*res=0;
	
	if ((x>=SF) || (x<0)){
		*res = ERROR_INPUT;
		return 0;
	}

	if((y>=SF) || (y<0)){
		*res = ERROR_INPUT;
	}
	
	return 0;
}

int add_ship(char str[5], char (*p)[SF][SF], int size, int* res)
{
	int start_x;
	int start_y;
	char ort;
	
	if(str==NULL) return 1;
	if(p==NULL) return 2;
	if(res==NULL) return 4;
	
	*res=0;
	
	if(convert_coords(str,&start_x,&start_y)!=0){ return 5;}
	
	if(check_coords(start_x, start_y, res)){ return 6;}
	
	if(*res!=0){
		return 0;
	}
	
	if(start_y>=9){
		ort=str[3];
	}
	else{
		ort=str[2];
	}
	
	if(size==1){
		ort='v';
	}
	
	if((ort!='v') && (ort!='h')){
		*res=ERROR_INPUT;
		return 0;
	}
	
	if(ort=='h'){
		if(start_x+size>SF){
			*res=ERROR_OF_FIELD_SIZE;
			return 0;
		}
	}
	else{
		if(start_y+size>SF){
			*res=ERROR_OF_FIELD_SIZE;
			return 0;
		}	
	}
	
	for(int i=0;i<size;i++)
	{
		for(int y=start_y-(start_y==0 ? 0 : 1);y<start_y+(start_y==SF-1 ? 1 : 2);y++)
		{
			for(int x=start_x-(start_x==0 ? 0 : 1);x<start_x+(start_x==SF-1 ? 1 : 2);x++)
			{
				if (i!=0)
				{
					if ((ort=='h') && (x==start_x-1) && (y==start_y))
					{
						continue;
					}
					if((ort=='v') && (y==start_y-1) && (x==start_x))
					{
						continue;
					}
				}
			
				if((*p)[y][x]=='@')
				{
					for(int j=0;j<i;j++)
					{
						if(ort=='h') {
							start_x--;
						}
						else{
							start_y--;
						}
						(*p)[start_y][start_x]='_';
					}
					*res=ERROR_SHIP_LOCATION;
					return 0;
				}
			}
		}
		
		(*p)[start_y][start_x]='@';
		
		if(ort == 'v'){
			start_y++;
		}
		
		if (ort == 'h'){
			start_x++;	
		}
	}
	
	return 0;
}
 

int check_ship(char field[SF][SF], int y, int x, int* res)
{
	if(field==NULL) return 1;
	if(res==NULL) return 4;
	
	char flag=0;
	*res=0;
	
	for(int i=y-(y==0 ? 0 : 1);i<y+(y==SF-1 ? 1 : 2);i++)
	{
		for(int j=x-(x==0 ? 0 : 1);j<x+(x==SF-1 ? 1 : 2);j++)
		{
			if(field[i][j]=='@'){
				flag=1;
				break;
			}	
		}
		if(flag!=0){
			*res=1;
			return 0;
		}
	}
		
	return 0;
}

int block_cells(char field[SF][SF], int y, int x)
{	
	if(field==NULL) return 1;
	
	for(int i=y-(y==0 ? 0 : 1);i<y+(y==SF-1 ? 1 : 2);i++)
	{
		for(int j=x-(x==0 ? 0 : 1);j<x+(x==SF-1 ? 1 : 2);j++)
		{
			if(field[i][j]=='_'){
				field[i][j]='%';
			}	
		}
	}
	
	return 0;	
}

int check_horizontal(char field[SF][SF], int y, int x)
{
	if(field==NULL) return 1;
	
	int res;
	int step=0;
		
		if(x!=0){
			step=-1;
			
			while(field[y][x+step]=='*'){				
				if(block_cells(field, y, x+step)!=0) return 5;
				step--;
			}	
		}
								
		if(x!=SF-1){
			step=1;
			
			while(field[y][x+step]=='*'){
				if(block_cells(field, y, x+step)!=0) return 6;
				step++;
			}	
		}
		return 0;
}

int	check_vertical(char field[SF][SF], int y, int x)
{
	int res;
	
	if(field==NULL) return 1;
	
	int step=0;
	
	if(y!=0){
		step=-1;
		
		while(field[y+step][x]=='*'){
			if(block_cells(field, y+step, x)!=0) return 5;
			step--;
		}	
	}
								
	if(y!=SF-1){
		step=1;
		
		while(field[y+step][x]=='*'){
			if(block_cells(field, y+step, x)!=0) return 6;
			step++;
		}	
	}
	
	return 0;	
}

int create_field_auto(char (*field)[SF][SF])
{
	int size=4;
	int col=1;
	int res=0;
	char coords[5] = {0};
	
	if(field==NULL) return 1;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<col;j++)
		{			
			if(generate_coords(&coords)!=0) return 5;
			
			if(add_ship(coords, field, size, &res)!=0) return 6;

			if(res!=0) j--;
		}
		
		size--;
		col++;
	}
	
	return 0;
}

int create_field(char (*field)[SF][SF])
{
	int size=4;
	int col=1;
	int res=0;
	char coords[5] = {0};
	char buf;
	
	if(field==NULL) return 1;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<col;j++)
		{
			if(system("cls")==-1) return 5;
			if(draw(*field, SHOW)!=0) return 6;
			if(printf("Введите координаты корабля: ") < 0) return 7;
			
			for(int i=0;i<5;i++){
				buf=getchar();
				if(buf!=0){
					if(buf!='\n'){
						coords[i]=buf;	
					}else{
						coords[i]=0; break;
					}
				}else{
					return 8;
				}
			}
			
			if(coords[4]!=0){
				while((buf=getchar())!='\n'){
					if(buf==0){
						return 9;
					}
				}
				coords[4]=0; 
			}
						
			if(add_ship(coords, field, size, &res)!=0) return 10;
			
			if(res!=0){
				switch(res){
					case 1:
						if(printf("Ошибка! Корабль выходит за границу игрового поля!\n") < 0) return 11;
						break;
					
					case 2:
						if(printf("Ошибка! Введены неверные координаты!\n") < 0) return 12;
						break;
						
					case 3:
						if(printf("Ошибка! Корабли не должны располагаться на смежных клетках!\n") < 0) return 13;
						break;
				}
				
				if(system("pause")==-1) return 14;
				if(system("cls")==-1) return 15;
				
				j--;
			}
		}
		
		size--;
		col++;
	}
	
	return 0;
}
