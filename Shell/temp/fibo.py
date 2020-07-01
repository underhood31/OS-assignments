import time

memo=[None]
def fibo(n):
    if n==0:
        return 0
    elif n==1:
        return 1 
    else:
        if memo[n]!=None:
            return memo[n]
        else:
            ans=fibo(n-1)+fibo(n-2)
            memo[n]=ans
            return ans

if __name__=="__main__":
    n=int(input("Enter a number: "))
    memo=[None]*(n+2)
    start_time=time.time()
    a=fibo(n)
    print("-->Time: ",time.time()-start_time)
    print("-->Ans: ",a) 
