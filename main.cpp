#include <iostream>
#include <string>
#include <podofo/podofo.h>


using namespace std;
using namespace PoDoFo;



void PrintHelp()
{
  std::cout << "This is a example application for the PoDoFo PDF library." << std::endl
  << "It creates a small PDF file containing the text >Hello World!<" << std::endl
  << "Please see http://podofo.sf.net for more information" << std::endl << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << "  examplehelloworld [outputfile.pdf]" << std::endl << std::endl;
}

void HelloWorld( const char* pszFilename ) 
{
  /*
   * PdfStreamedDocument is the class that can actually write a PDF file.
   * PdfStreamedDocument is much faster than PdfDocument, but it is only
   * suitable for creating/drawing PDF files and cannot modify existing
   * PDF documents.
   *
   * The document is written directly to pszFilename while being created.
   */
  PdfStreamedDocument document( pszFilename );
  
  /*
   * PdfPainter is the class which is able to draw text and graphics
   * directly on a PdfPage object.
   */
  PdfPainter painter;
  
  /*
   * This pointer will hold the page object later. 
   * PdfSimpleWriter can write several PdfPage's to a PDF file.
   */
  PdfPage* pPage;
  
  /*
   * A PdfFont object is required to draw text on a PdfPage using a PdfPainter.
   * PoDoFo will find the font using fontconfig on your system and embedd truetype
   * fonts automatically in the PDF file.
   */     
  PdfFont* pFont;
  
  PdfImage pImage (&document);
  
  try {
    /*
     * The PdfDocument object can be used to create new PdfPage objects.
     * The PdfPage object is owned by the PdfDocument will also be deleted automatically
     * by the PdfDocument object.
     *
     * You have to pass only one argument, i.e. the page size of the page to create.
     * There are predefined enums for some common page sizes.
     */
    pPage = document.CreatePage( PdfPage::CreateStandardPageSize( ePdfPageSize_Letter, true ) ); //ePdfPageSize_Letter, ePdfPageSize_A4. Landscape:true/false
    
    /*
     * If the page cannot be created because of an error (e.g. ePdfError_OutOfMemory )
     * a NULL pointer is returned.
     * We check for a NULL pointer here and throw an exception using the RAISE_ERROR macro.
     * The raise error macro initializes a PdfError object with a given error code and
     * the location in the file in which the error ocurred and throws it as an exception.
     */
    if( !pPage ) 
    {
      PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
    }
    
    /*
     * Set the page as drawing target for the PdfPainter.
     * Before the painter can draw, a page has to be set first.
     */
    painter.SetPage( pPage );
    
    /*
     * Create a PdfFont object using the font "Arial".
     * The font is found on the system using fontconfig and embedded into the
     * PDF file. If Arial is not available, a default font will be used.
     *
     * The created PdfFont will be deleted by the PdfDocument.
     */
    pFont = document.CreateFont( "Arial" );
    
    /*
     * If the PdfFont object cannot be allocated return an error.
     */
    if( !pFont )
    {
      PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
    }
    
    /*
     * Set the font size
     */
    pFont->SetFontSize( 18.0 );
    
    /*
     * Set the font as default font for drawing.
     * A font has to be set before you can draw text on
     * a PdfPainter.
     */
    painter.SetFont( pFont );
    
    /*
     * You could set a different color than black to draw
     * the text.
     *
     * SAFE_OP( painter.SetColor( 1.0, 0.0, 0.0 ) );
     */
    
    /*
     * Actually draw the line "Hello World!" on to the PdfPage at
     * the position 2cm,2cm from the top left corner. 
     * Please remember that PDF files have their origin at the 
     * bottom left corner. Therefore we substract the y coordinate 
     * from the page height.
     * 
     * The position specifies the start of the baseline of the text.
     *
     * All coordinates in PoDoFo are in PDF units.
     * You can also use PdfPainterMM which takes coordinates in 1/1000th mm.
     *
     */
    
    //PoDoFo have no margins methods, thus we establish margins manually
    double leftMargin=20;
    double rigthMargin=20;
    double topMargin=20;
    double bottomMargin=30;
    
    //Set font size
    pFont->SetFontSize( 08.0 );
    painter.SetFont( pFont );
    
    //Use GetFontMetrics to get width and heigth of a single text line
    string msgText="Some text here.";
    double textWidth=pFont->GetFontMetrics()->StringWidth( msgText );
    double textHeight=pFont->GetFontMetrics()->GetLineSpacing();
    painter.DrawText( pPage->GetPageSize().GetWidth() - rigthMargin - textWidth, bottomMargin + textHeight, msgText );
    
    //Draw a rectangle around the text
    painter.Rectangle( pPage->GetPageSize().GetWidth() - rigthMargin - textWidth, bottomMargin + textHeight-3, textWidth, textHeight );
    painter.Stroke();

    //Use DrawTextAligned to center a text
    pFont->SetFontSize( 18.0 );
    painter.SetFont( pFont );
    painter.DrawTextAligned( leftMargin, (pPage->GetPageSize().GetHeight()-topMargin) - 20,  pPage->GetPageSize().GetWidth() - (leftMargin + rigthMargin) , "Title of This Page", ePdfAlignment_Center );
    
    
    pFont->SetFontSize( 11.0 );
    painter.SetFont( pFont );
    
    //Create a long text and use DrawMultiLineText for writing a parragraph
    string rsText="A parragraph a parragraph a parragraph a parragraph a parragraph a parragraph a parragraph. ";
    rsText+=rsText;rsText+=rsText;rsText+=rsText;rsText+=" End";
    
    double boxWidth=pPage->GetPageSize().GetWidth() - (leftMargin + rigthMargin);
    double boxHeight=150;
    painter.DrawMultiLineText (leftMargin, (pPage->GetPageSize().GetHeight() - topMargin - boxHeight) - 30, boxWidth, boxHeight, rsText, ePdfAlignment_Left, ePdfVerticalAlignment_Top);
    
    //Create an image in the pdf from a image file
    pImage.LoadFromFile("image.jpg");
    
    double scaleX=0.5;
    double scaleY=0.5;
    painter.DrawImage( leftMargin + ( pPage->GetPageSize().GetWidth() - pImage.GetWidth()*scaleX - (leftMargin + rigthMargin) ) / 2, bottomMargin, &pImage, scaleX ,scaleY );
    
    
    /*
     * Tell PoDoFo that the page has been drawn completely.
     * This required to optimize drawing operations inside in PoDoFo
     * and has to be done whenever you are done with drawing a page.
     */
    painter.FinishPage();
    
    /*
     * Set some additional information on the PDF file.
     */
    document.GetInfo()->SetCreator ( PdfString("examplahelloworld - A PoDoFo test application") );
    document.GetInfo()->SetAuthor  ( PdfString("Dominik Seichter") );
    document.GetInfo()->SetTitle   ( PdfString("Hello World") );
    document.GetInfo()->SetSubject ( PdfString("Testing the PoDoFo PDF Library") );
    document.GetInfo()->SetKeywords( PdfString("Test;PDF;Hello World;") );
    
    /*
     * The last step is to close the document.
     */
    document.Close();
  } catch ( const PdfError & e ) {
    /*
     * All PoDoFo methods may throw exceptions
     * make sure that painter.FinishPage() is called
     * or who will get an assert in its destructor
     */
    try {
      painter.FinishPage();
    } catch( ... ) {
      /*
       * Ignore errors this time
       */
    }
    
    throw e;
  }
}



int main(int argc, char **argv) {
  /*
   * Check if a filename was passed as commandline argument.
   * If more than 1 argument or no argument is passed,
   * a help message is displayed and the example application
   * will quit.
   */
  if( argc != 2 )
  {
    PrintHelp();
    return -1;
  }
  
  /*
   * All podofo functions will throw an exception in case of an error.
   *
   * You should catch the exception to either fix it or report
   * back to the user.
   *
   * All exceptions podofo throws are objects of the class PdfError.
   * Thats why we simply catch PdfError objects.
   */
  try {
    /*
     * Call the drawing routing which will create a PDF file
     * with the filename of the output file as argument.
     */
    HelloWorld( argv[1] );
  } catch( const PdfError & eCode ) {
    /*
     * We have to check if an error has occurred.
     * If yes, we return and print an error message
     * to the commandline.
     */
    eCode.PrintErrorMsg();
    return eCode.GetError();
  }
  
  
  /**
   * Free global memory allocated by PoDoFo.
   * This is normally not necessary as memory
   * will be free'd when the application terminates.
   * 
   * If you want to free all memory allocated by
   * PoDoFo you have to call this method.
   * 
   * PoDoFo will reallocate the memory if necessary.
   */
  PdfEncodingFactory::FreeGlobalEncodingInstances();
  
  /*
   * The PDF was created sucessfully.
   */
  std::cout << std::endl
  << "Created a PDF file containing the line \"Hello World!\": " << argv[1] << std::endl << std::endl;
  
  
  return 0;
}
