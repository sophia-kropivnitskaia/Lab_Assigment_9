#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType* record;
    struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	return x % 31;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		struct HashType *node = &pHashArray[i];
		if (node->record != NULL) {
			printf("(%d, %c, %d) -> ", node->record->id, node->record->name, node->record->order);
		}
		node = node->next;
		while (node != NULL) {
			printf("(%d, %c, %d) -> ", node->record->id, node->record->name, node->record->order);
			node = node->next;
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
	// Your hash implementation
	const int n = 31;
    struct HashType *hashArr = (struct HashType *)malloc(sizeof(struct HashType) * n);
    for (int i = 0; i < n; ++i)
    {
        hashArr[i].record = NULL;
		hashArr[i].next = NULL;
    }

    // Insert records into hash structure
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
		if (hashArr[index].record == NULL) {
			hashArr[index].record = &pRecords[i];
		}
		else {
			struct HashType *node = &hashArr[index];
			while (node->next != NULL) {
				node = node->next;
			}
			node->next = (struct HashType *)malloc(sizeof(struct HashType));
			node = node->next;
			node->record = &pRecords[i];
			node->next = NULL;
		}
    }

    // Display records in the hash structure
    displayRecordsInHash(hashArr, n);

    // Free allocated memory
    free(pRecords);
 	for (int i = 0; i < n; ++i) {
		struct HashType *node = hashArr[i].next;
		while (node != NULL) {
			struct HashType *next = node->next;
			free(node);
			node = next;
		}
	}
	free(hashArr);

    return 0;
}