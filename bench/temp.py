import time

def sieve(n):
    prime = []
    is_composite = [False] * (n + 1)
    
    for i in range(2, n + 1):
        if not is_composite[i]:
            prime.append(i)
        for j in range(len(prime)):
            if i * prime[j] > n:
                break
            is_composite[i * prime[j]] = True
            if i % prime[j] == 0:
                break
    
    return prime

start_time = time.time()
print(sieve(1000000))
end_time = time.time()

print(f"Execution time: {end_time - start_time} seconds")
