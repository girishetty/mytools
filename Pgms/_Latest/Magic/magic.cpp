#include <iostream.h>
#include <conio.h>

void main()
{
	__int64 x=9449596313350072,y=0,z=9423945333333333;

	for(y=2;y<100000000;y++)
	{
		if(z%2==0)
		{			
			if(z%4==0)
				{
					if(z%8==0)
						{
							if(z%11==0)
							{
								if(z%22==0)
									{	
									if(z%44==0)
										{
											if(z%47==0)
												{
													if(z%61==0)
														{
															if(z%71==0)
															{
																if(z%88==0)
																{
																	if(z%94==0)
																	{
																	cout<<"1."<<(long double)y<<endl<<(long double)z;
																	}
																}
															}
														}
												}
										}
									}

							}
						}	
				}
			}
										
				
		z++;
	}
}
