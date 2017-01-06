
/**
 * Linear search of the array
 *
 * @param key The search key
 * @param array[] The array to search
 * @param size The size of the array
 * @return The index of the key or -1 if it can't be found
 */
int lsearch(int key, int array[], int size)
{
    for (int i = 0; i < size; i++) {
        if (array[i] == key) {
            return i;
        }
    }

    return -1;
}


/**
 * Function to return the address of the found element
 * This will work well for primitive types but will not work well
 * for strings, or structs that contain pointers.
 * 
 * @param key Pointer to the search key
 * @param base Pointer to the base of the array
 * @param n The number of elements in the array
 * @param elemSize The size in bytes of the elements
 * @return Pointer to the found element's address
 */
void *lsearch(void *key, void *base, int n, int elemSize)
{
    for (int i = 0; i < n; i++) {
        // Each iteration I need to compute the address of the i-th element
        void *elemAddr = (char *) base + i * elemSize; // The void * hack
        if (memcmp(key, elemAddr, elemSize) == 0) {
            return elemAddr;
        }
    }

    return NULL;
}

void *lsearch(void *key, void *base, int n, int elemSize, int (*cmpfn)(void *, void *))
{
    for (int i = 0; i < n; i ++) {
        void *elemAddr = (char *)base + i * elemSize;
        if (cmpfn(key, elemAddr) == 0) {
            return elemAddr;
        }
    }
    
    return NULL;
}

// Compare functions that a client would create to statify the generic lsearch routine

/**
 * Satisfies the cmpfn prototype required by lsearch
 * @example
 *  int key = 7;
 *  int array[] = { 1, 4, 8, 7, 5, 2 };
 *  int size = 6;
 *  int *found = lsearch(&key, array, size, sizeof(int), intcmp);
 *
 *  if (found == NULL) :(
 *  else :)
 */
int intcmp(void *expected, void *actual)
{
    // Because the client is implementing this compare routine,
    // I can set the params to the type that they really are -- int *
    int *exp = expected; // Compiler implicitly casts to type int *
    int *act = actual;

    // If the difference is 0 then they are the same number
    return *exp - *act;
}

/**
 * This prototype of cmpfn is not type safe but the client implementation
 * of that routine can become type safe if I cast everything properly.
 *
 * @example
 *  char *key = "Eb";
 *  char *notes[] = { "Ab", "F#", "B", "Gb", "D" };
 *  int size = 5; // size of the array
 *  char **found = lsearch(&key, notes, size, sizeof(char *), StrCmp);
 */
int StrCmp(void* expected, void *actual)
{
    // Cast expected and actual to the types I know they are.
    // After casting dereference the values one time.
    // Why not just cast to a char *?
    // char ** is the actual address of the value that is expected.
    // We don't want the address of the actuall char array, we need
    // to jump forward one byte to get to the pointer of the first
    // char in the c strings we want to compare. Dereferencing will
    // jump us forward one byte.
    char *s1 = *(char **)expected;
    char *s2 = *(char **)actual;

    // Pass the buck to the clib strcmp function
    return strcmp(s1, s2);
}

