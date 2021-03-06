
#include <stdio.h>


#include "../common/define.h"
#include "../common/DocumentIndex.h"


int main (int argc, char *argv[]) {

	struct DocumentIndexFormat DocumentIndexPost;
	int LotNr;
	unsigned int DocID;
	FILE *FH;

        if (argc < 3) {
                printf("Dette programet leser en DocumentIndex. Gi det et lot nr og ut fil for nye urler. \n\n\tUsage: ./readDocumentIndex 1 /temp/udfile");
               exit(0);
        }

	LotNr = atoi(argv[1]);

        if ((FH = fopen(argv[2],"ab")) == NULL) {
                perror(argv[2]);
                exit(1);
        }
	
	while (DIGetNext (&DocumentIndexPost,LotNr,&DocID)) {

		if (
			(DocumentIndexPost.clientVersion < 0.79) 
			&& (strlen(DocumentIndexPost.Url) != 0)
			&& (strstr(DocumentIndexPost.Url,".no/") != 0) 
		) {
			//printf("DocID: %u, url: %s, v: %f\n",DocID,DocumentIndexPost.Url,DocumentIndexPost.clientVersion);

			fwrite(DocumentIndexPost.Url,200,1,FH);
			fwrite(&DocID,sizeof(int),1,FH);

		}
	}

	fclose(FH);
	DIClose();


}

