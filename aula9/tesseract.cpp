#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main(int argc, char *argv[])
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library    
    Pix *image = pixRead(argv[1]);
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("%s", outText);

    // Destroy used object and release memory
    api->End();
    delete api;
    delete [] outText;
    pixDestroy(&image);

    return 0;
}