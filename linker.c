// Linker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// globals
int pcounter_base = 0; // base program counter for each file
int pcounter_base_next = 0;
char sym_buffer[4096]; // buffer for symbols
int pos = 0;		   // position in sym_buffer
int length = 0;		   // written length
FILE *outFile;

//Search the whole buffer to find the address of the label
int search_sym_buffer(char *label)
{
	int address;
	int off;
	char entry[256];
	char *buf = sym_buffer;

	while (sscanf(buf, "%s %d %n", entry, &address, &off) == 2)
	{
		buf += off;
		if (strcmp(label, entry) == 0)
		{
			return address;
			fprintf(stderr, "Label %s at address %d\n", label, address);
		}
	}
	fprintf(stderr, "Label not found %s\n", label);
	return 0;
}

// Processes a single file and writes to outFile. Depending on oflag,
// it is either a stdout or a tempfile before a final linkage.
void ProcessSingleFile(FILE *file)
{
	char line[256];
	char instruction[256];
	char label[256];
	int pc;
	int found;

	if (file == NULL)
		return;
	while (fgets(line, 256, file) != NULL)
	{
		found = sscanf(line, "%d %s %s", &pc, instruction, label);
		if (found < 2)
		{
			if (sscanf(line, "%s %d", label, &pc) == 2)
			{
				length = sprintf(sym_buffer + pos, "%s %d\n", label, pcounter_base + pc);
				pos += length;
				//printf("%s %d\n", label, pc);
			}
			continue;
		}
		if ((found == 2) &&
			(pc == 4096) && (strcmp(instruction, "x") == 0))
			continue;
		if (found == 2)
			fprintf(outFile, "%d %s\n", pcounter_base + pc, instruction);
		if (found == 3)
			fprintf(outFile, "%d %s %s\n", pcounter_base + pc, instruction, label);
		pcounter_base_next = pcounter_base + pc + 1;
	}
	pcounter_base = pcounter_base_next;
}

void FinalLinkage(FILE *file)
{

	char line[256];
	char instruction[256];
	char label[256];
	int pc;
	int found;
	int line_num = 0;

	if (file == NULL)
		return;

	while (fgets(line, 256, file) != NULL)
	{
		found = sscanf(line, "%d %s %s", &pc, instruction, label);
		if (found < 2)
			continue;
		if ((found == 2) &&
			(pc == 4096) && (strcmp(instruction, "x") == 0))
			continue;
		while (pc != line_num)
		{
			// data space, print extra
			printf("%s\n", "1111111111111111");
			line_num++;
		}
		if (found == 2)
		{
			printf("%s\n", instruction);
		}
		if (found == 3)
		{
			if (instruction[0] == 'U')
			{
				// Resolve references	srgt5
				char resolved_instruction[17];
				for (int i = 0; i < 4; i++)
				{
					resolved_instruction[i] = instruction[i + 1];
				}
				int n = search_sym_buffer(label);
				for (int c = 0; c < 12; c++)
				{
					int k = n >> c;
					if (k & 0x1)
						resolved_instruction[15 - c] = '1';
					else
						resolved_instruction[15 - c] = '0';
				}
				resolved_instruction[16] = '\0';
				printf("%s\n", resolved_instruction);
			}
		}
		line_num++;
	}
}

int main(int argc, char *argv[])
{

	char line[256];
	char instruction[256];
	char label[256];
	int i;
	int pc;
	int main_idx = -1; // index of a file containing main()
	int file_idx;
	int num_files;
	bool oflag = false;

	if (argc < 2)
		return -1;

	if (strcmp(argv[1], "-o") == 0)
	{
		oflag = true;
		file_idx = 2;
		outFile = stdout;
	}
	else
	{
		outFile = fopen("/tmp/linker.out", "w");
		if (outFile == NULL)
		{
			printf("Cannot open temp file");
			exit(-1);
		}
		file_idx = 1;
	}
	num_files = argc - file_idx;
	FILE *files[num_files];

	// open files
	for (i = 0; i < num_files; i++)
	{
		files[i] = fopen(argv[i + file_idx], "r");
		if (files[i] == NULL)
		{
			printf("Cannot open file %s \n", argv[i + file_idx]);
			exit(-1);
		}
	}

	// first find the file with main()
	for (i = 0; i < num_files; i++)
	{
		while (fgets(line, 256, files[i]) != NULL)
		{
			if ((sscanf(line, "%s", label) == 1) &&
				(strcmp(label, "main:") == 0))
			{
				main_idx = i;
				break;
			}
		}
	}

	if (main_idx == -1)
	{
		fprintf(stderr, "No main() function found \n");
		exit(-1);
	}

	// rewind
	for (i = 0; i < num_files; i++)
	{
		rewind(files[i]);
	}

	// process main first
	ProcessSingleFile(files[main_idx]);
	// process other files
	for (i = 0; i < num_files; i++)
	{
		if (i == main_idx)
			continue;
		ProcessSingleFile(files[i]);
	}
	fprintf(outFile, "4096 x\n");
	// Now print the symbol table
	fprintf(outFile, "%s", sym_buffer);

	// close files
	for (i = 0; i < num_files; i++)
	{
		fclose(files[i]);
	}
	if (!oflag)
	{
		fflush(outFile);
		fclose(outFile);
		// open as read-only
		outFile = fopen("/tmp/linker.out", "r");
		if (outFile == NULL)
		{
			printf("Cannot open temp file");
			exit(-1);
		}
		FinalLinkage(outFile);
	}
	return 0;

} // end main
