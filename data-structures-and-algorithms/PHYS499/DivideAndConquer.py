
def Merge(B,C,A):
    i,j,k=0,0,0
    while i<len(B) and j<len(C):
        if B[i]<=C[j]:
            A[k]=B[i]
            i+=1
        else:
            A[k]=C[j]
            j+=1
        k+=1
    if i==len(B):
        A[k:]=C[j:]
    else:
        A[k:]=B[i:]

def Mergesort(A,n):
    if n>=1:
        B=A[:n]
        C=A[n:]
        Mergesort(B,n/2)
        Mergesort(C,n/2)
        Merge(B,C,A)


def partition(A,l,r):
    p=A[r]
    i=l-1
    j=l
    while j<r:
        if A[j]<=p:
            i+=1
            A[i],A[j]=A[j],A[i]       
        j+=1
    A[i+1],A[r]=A[r],A[i+1]
    return i+1
def quicksort(A,l,r):
    if l<r:
        s=partition(A,l,r)
        quicksort(A,l,s-1)
        quicksort(A,s+1,r)


def BinarySearch(A,K):
    i=0
    r=len(A)
    while i<=r:
        m=(i+r)/2
        if K==A[m]:
            return m
        elif K<A[m]:
            r=m-1
        else:
            i=m+1
    return -1


def BinarySearchRecur(A,K):
    i=0; r=len(A)-1
    def Binary2(A,K,i,r):
        m=(i+r)/2
        if K==A[m]:
            return m
        elif K<A[m]:
            return Binary2(A,K,i,m-1)
        else:
            return Binary2(A,K,m+1,r)
    if i>r:
        return False
    else:
        return Binary2(A,K,i,r)


