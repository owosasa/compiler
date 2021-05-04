没有实现的语法特性（或许有疏漏）：

高维数组，逻辑与或非，数组表达式下标访问，标识符作用域（函数共享标识符），全局变量，不能使用数组函数参数，CONTINUE，结构体，注释。



示例程序（欧拉筛法）：

```C
int main() {
	int vis[33], pri[33], pos, iter, temp, cnt = 0;
	for (pos = 0; pos < 33; pos++) { vis[pos] = 0; pri[pos] = 0; }
	vis[0] = 1; vis[1] = 1;
	for (pos = 2; pos < 33; pos++) {
		if (vis[pos] == 0) { pri[cnt] = pos; cnt++;	}
		for (iter = 0; iter < cnt; iter++) {
			if (pos * pri[iter] > 33) {	break; }
			temp = pos * pri[iter];
			vis[temp] = 1;
			temp = pos % pri[iter];
			if (temp == 0) { break;	}
		}
	}
}
```

示例程序（递归阶乘）：

```C
int Function(int A) {
	int res = A, next = A - 1, sub = 1;
	if (next > 0) {
		sub = Function(next); 
	}
	res = res * sub;
	return res;
}
int main() {
	int arr[7] = { 1,2,3,4,5,6,7 }, pos;
	for (pos = 0; pos < 7; pos++) {
		arr[pos] = Function(arr[pos]);
	}
}
```



DOSbox 0.74，masm环境在ASM目录下