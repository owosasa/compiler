该编译器为C实现的C编译器子集（C程序转换为汇编）

汇编环境：DOSbox 0.74，masm环境在ASM目录下



没有实现的语法特性（应该有疏漏）：

高维数组，逻辑与或非，数组表达式下标访问，标识符作用域（函数内共享标识符），全局变量，不能使用数组作为函数参数，结构体，注释，CONTINUE，SWITCH。



词法分析状态转移设计如下图，语法分析基于C0文法。

![](https://cdn.luogu.com.cn/upload/image_hosting/97m9b8p6.png)



示例程序（欧拉筛法）：

```C
int main() {
	int vis[33], pri[33], pos, iter, temp, cnt = 0;
	for (pos = 0; pos < 33; pos++) { 
		vis[pos] = 0; pri[pos] = 0; 
	}
	vis[0] = 1; vis[1] = 1;
	for (pos = 2; pos < 33; pos++) {
		if (vis[pos] == 0) { 
			pri[cnt] = pos; cnt++; 
		}
		for (iter = 0; iter < cnt; iter++) {
			if (pos * pri[iter] > 33) { 
				break; 
			}
			temp = pos * pri[iter];
			vis[temp] = 1;
			temp = pos % pri[iter];
			if (temp == 0) { 
				break;	
			}
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



示例程序（欧拉筛法）生成汇编并在DOSbox下运行结果如下图所示：

![](https://cdn.luogu.com.cn/upload/image_hosting/u4ehqgez.png)

33 内的质数有 0BH 个：02H，03H，05H，07H，0BH，0DH，11H，13H，17H，1DH，1FH。



示例程序（递归阶乘）生成汇编并在DOSbox下运行结果如下图所示：

![](https://cdn.luogu.com.cn/upload/image_hosting/mravecq9.png)

运行结束后数组 arr 的值（即1~7的阶乘）分别为：01H，02H，06H，18H（24D），78H（120D），02D0H（720D），13B0H（5040D）。

