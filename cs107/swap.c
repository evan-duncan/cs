
/**
 * Lecture 3/4 working on a generic swap in c
 */

// This can only swap ints and does not 
// allow swapping of structs, doubles, etc.
void swap(int *ap, int *bp)
{
    int tmp = *ap;
    *ap = *bp;
    *bp = tmp;
}

/**
 * Generic swap using generic pointers.
 * @param *ap  Address of pointer A
 * @param *bp  Address of pointer B
 * @param size The number of bytes for a and b
 * @example
 *   int x = 10, y = 15;
 *   swap(&x, &y, sizeof(int));
 */
void swap(void *ap, void *bp, int size)
{
    char buffer[size];
    memcpy(buffer, ap, size); // generically copy bytes from ap into buffer for the given size
    memcpy(ap, bp, size);
    memcpy(bp, buffer, size);
}
