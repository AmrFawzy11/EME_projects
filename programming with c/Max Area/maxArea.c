int mnm(int a , int b){
	if(a<b)
	{
	  return a ;
	}
	return b ;
}

int maxArea(int len , int* arr){
	int mxmArea = len * mnm(arr[0], arr[len-1]) , area = 0;
	int i=0 , j= len-1 ;
 	while(i<j)
	{
		if(arr[i] < arr[j])
			++i;
		else
			--j;
		area = (j-i)*mnm(arr[i],arr[j]);
		if(area > mxmArea)
			mxmArea = area ;
	}
	return mxmArea ;
}
