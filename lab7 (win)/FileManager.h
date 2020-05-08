#include <fstream>
#include <string.h>
#include <windows.h>

using namespace std;

namespace filemanager
{
	class FileManager
	{
	public:
		typedef char byte;
		static const char FHYSICAL_SPACE_NAME[];
		static const int FILENAME_LENGTH = 50;
		static const int FILES_QUANTITY = 100;
		static const int BLOCK_DATA_SIZE = 50;
		static const int BLOCKS_QUANTITY = 2500; 

		struct FileMetaDataTableField
		{
			int initialOffset = 0;
			char fileName[FILENAME_LENGTH] = { 0 };
		};

		// контейнер для дескрипторов файлов
		typedef FileMetaDataTableField* FileMetaDataTable;

		struct MemoryBlock
		{
			byte busyFlag = 0; // флаг занятости
			int nextOffset = 0; // дальнейшее смещение
			char fileData[BLOCK_DATA_SIZE] = { 0 };
		};

		void AddFile();
		void ShowFile();
		void OpenFile();
		void HelpInfo();
		void ResetSpace();
		void DeleteFile();

		FileManager();
		~FileManager();

		static const int FIRST_BLOCK_OFFSET = FILES_QUANTITY * sizeof(FileMetaDataTableField) + 1;
		static const int FMDT_SIZE = FIRST_BLOCK_OFFSET - 1;

	private:
		int freeSpot;
		fstream sourceFile;
		int FindFreeBlock(bool next);
		void SaveFileMetaDataTable();
		string ProcessFileBuffer(string);
		MemoryBlock ReadMemoryBlock(int offset);
		void ClearMemoryBlock(MemoryBlock& block);
		void SaveFile(string, int beginingOffset);
		FileMetaDataTableField FMDTField[FILES_QUANTITY];
		string ReadFileToBufferString(int beginingOffset);
		void SaveMemoryBlock(MemoryBlock block, int offset);
		void CopyString(char* destinantion, int destinationMaxSize, const char* source);
	};
}
