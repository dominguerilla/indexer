/*JIMMY LE 				jl1415
 *CARLOS DOMINGUEZ		cmd363
 *	index.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "tokenizer.h"

typedef struct FNode FNode;

struct FNode
{
	char *file;
	int count;
	FNode *next;
};

typedef struct WNode WNode;

struct WNode
{
	char *word;
	WNode *next;
	FNode *fHead;
};


static WNode *head = NULL;
static char delims[100];

int isDirectory(char *path) {
	struct stat buf;
	stat(path, &buf);
	return S_ISDIR(buf.st_mode);
}

void clear()
{
	WNode *cur = head;
	while (cur != NULL)
	{
		WNode *temp = cur;
		free(temp->word);

		FNode *fCurrent = temp->fHead;
		while (fCurrent != NULL)
		{
			FNode *ftemp = fCurrent;
			free(ftemp->file);
			fCurrent = ftemp->next;
			free(ftemp);
		}
		cur = temp->next;
		free(temp);
	}
}

void insertLL(WNode *node)
{
	if (head == NULL)
	{
		head = node;
		return;
	}

	WNode *prev = NULL;
	WNode *cur = head;
	while (cur != NULL)
	{
		int cmp = strcmp(cur->word, node->word);
		if (cmp > 0) 
		{ 
			if (prev == NULL) { 
				node->next = head;
				head = node;
			} else {
				node->next = cur;
				prev->next = node;
			}
			return; 
		}
		prev = cur;
		cur = cur->next;
	}
	prev->next = node;
}


int evalFile(char *path)
{
	char *buffer = NULL;	
	FILE *filestream = fopen(path, "r");

	if (filestream == NULL)
	{
		fprintf(stderr, "ERROR: Unreadable File: \"%s\"\n", path);
		/*perror("");*/
		return 0;
	}
	fseek(filestream, 0, SEEK_END);

	long fsize = ftell(filestream);

	fseek(filestream, 0, SEEK_SET);

	buffer = malloc(fsize+1);

	if (buffer == NULL) {
		printf("ERROR: Unable to malloc memory space\n");
		fclose(filestream);
		return 0;
	}else if (fread(buffer, fsize, 1, filestream) == 0) {
		fprintf(stderr, "ERROR: Unreadable File \"%s\"\n", path);
		free(buffer);
		fclose(filestream);
		return 0;
	}

	buffer[fsize] = '\0';
	fclose(filestream);

	TokenizerT *tokenizer = TKCreate(delims, buffer);

	if (tokenizer == NULL) {
		fprintf(stderr, "ERROR: Unable to tokenize\n");
		return 0;
	}

	char * token = NULL;

	while((token = TKGetNextToken(tokenizer)) != NULL) 
	{
		int i;
		for (i = 0; i < strlen(token); i++) {
			token[i] = tolower(token[i]);
		}
		WNode *nodeW = NULL;
		FNode *nodef = NULL;

		
		if ( head != NULL )
		{
			WNode *cur = head;
			while (cur != NULL)
			{
				if (strcmp(cur->word, token) == 0) { 
					nodeW = cur;
					break;
				}
				cur = cur->next;
			}
		}

		
		if ( head == NULL || nodeW == NULL)
		{
			nodeW = (WNode *) malloc(sizeof(WNode));
			nodef = (FNode *) malloc(sizeof(FNode));
			nodef->count = 1;
			nodef->next = NULL;
			nodef->file = strdup(path);
			nodeW->word = strdup(token);
			nodeW->fHead = nodef;
			nodeW->next = NULL;
			insertLL(nodeW);
		} else 
		{ 
			
			FNode *prev = NULL;
			FNode *fCurrent = nodeW->fHead;
			FNode *new = NULL; 

			int addNode = 0; 

			while (fCurrent != NULL)
			{
				int cmp = strcmp(path, fCurrent->file);
				if (cmp == 0) { 
					(fCurrent->count)++;
					addNode = 1;
					break;
				} else if (cmp > 0) 
				{
					new = (FNode *) malloc(sizeof(FNode));
					new->file = strdup(path);
					new->count = 1;
					new->next = fCurrent;
					if (prev == NULL) { 
						nodeW->fHead = new;
					} else {
						prev->next = new;
					}
					addNode = 1;
					break;
				}

				prev = fCurrent;
				fCurrent = fCurrent->next;
			}
			
			if (addNode == 0)
			{
				new = (FNode *) malloc(sizeof(FNode));
				new->file = strdup(path);
				new->count = 1;
				new->next = NULL;
				prev->next = new;
			}
		}
		free(token);
	}

	TKDestroy(tokenizer);
	free(buffer);
	
	return 1;
}


int writeFile(char *file)
{
	WNode *cur = head;
	if (cur == NULL) {
		printf("ERROR: Empty File - \"%s\"\n", file);
		return 1;
	}

	FILE *fd = fopen(file, "w");

	if (fd == NULL) {
		fprintf(stderr, "ERROR: Unable to write \"%s\"\n", file);
		return 0;
	}

	fprintf(fd,"{\"list\" : [\n");
	
	while (cur != NULL)
	{
		fprintf(fd, "\t\t{\"%s\" : [\n", cur->word);
		int count = 1;
		FNode *fCurrent = cur->fHead;
		while (fCurrent != NULL)
		{
			if (count == 1) {
				fprintf(fd, "\t\t\t\t{\"%s\" : %d}\n", fCurrent->file, fCurrent->count);
			} else {
				fprintf(fd, "\t\t\t\t{\"%s\" : %d},\n", fCurrent->file, fCurrent->count);
			}
			fCurrent = fCurrent->next;
			if (count++ == 5 && fCurrent != NULL) {
				fprintf(fd, "\n");
				count = 1;
			}
		}
		fprintf(fd, "\t\t]}\n");
		cur = cur->next;
	}
	fprintf(fd,"]}\n");

	fclose(fd);
	return 1;
}

void evalDir(char *path)
{
	DIR *dirFD;
	struct dirent *ent;
	if ((dirFD = opendir(path)) != NULL) {
		while ((ent = readdir(dirFD)) != NULL) 
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
				continue;
			}

			int lendir = strlen(path); 
			int lenfile = strlen(ent->d_name); 
			char buffer[lendir+lenfile+1]; 
			memcpy(buffer, path, lendir);
			if (path[lendir-1] == '/') {
				memcpy(buffer+lendir, ent->d_name, lenfile+1);
			} else {
				buffer[lendir] = '/';
				memcpy(buffer+lendir+1, ent->d_name, lenfile+1);
			}

			if (isDirectory(buffer) == 1)
			{
				evalDir(buffer);
			} else {
				evalFile(buffer); 
			}
		}
		closedir(dirFD);
	} else {
		printf("ERROR: Unable to open directory\n");
		return;
	}
}


