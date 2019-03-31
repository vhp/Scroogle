/*

nbbw.c     January 3, 2005    Scroogle Scraper

Released to the public domain for nonprofit use by
Public Information Research, Inc., San Antonio, Texas.

The latest version of this file is at http://www.scroogle.org/zipdir/nbbw.zip
If you found it somewhere else, you'll want to make sure you have the
latest version.

What is the legal situation with scraping Google?

   Google provides a version of their main index to the public that is
free of ads. This is at www.google.com/ie on all of their data centers.
It is apparently used by some versions of Explorer for some feature.
The point is, Google provides this for public use, and it is listed in the
Google Directory as a "bare bones interface that returns only page titles."
(The snippets are there but you don't see them until you mouse-over in
Explorer.) This version of Google is quite stable and easy to scrape.
Nothing has changed on the formatting of these pages for 2.5 years.
Therefore, we contend that the ad-free results offered by this scraper do
not do anything that Google itself does not do. We merely make their /ie
interface more attractive and useful.

   Public Information Research, Inc. is a 501(c)(3) nonprofit, tax-exempt
public charity with annual revenue of under $20,000. We ran this proxy
openly on Google Watch for two years without any complaints from Google.
However, our Scroogle site at one point was running a side-by-side filter
demonstration that was blocked by Google in December, 2003. That was
different from this program. This one is the straight proxy that began
running in September, 2002 on Google Watch. Within one year, we averaged
about 5,000 searches per day.

When Google blocked Scroogle, the Scroogle program was switched to a
different server. One characteristic of that server was that the IP address
that showed up at Google when we used curl or wget to fetch a search, just
happened to be on a different Class C than all of our domains on that
dedicated server. This meant that Google could no longer block us merely
by blocking the Class C that they knew about. Instead, they'd have to do
a huge number of experiments to discover our address. That's how we went
around Google's blocking. We never heard from Google's lawyers; they never
complained to us even though they knew about us and knew where to find us.

   Legally, Google probably has the right to block anyone they want. And
legally, we believe that as a tiny nonprofit with an interest in Google's
violations of privacy, we have the right to access Google's publicly-
available data any way we want. If you want to argue about copyright, then
let's start with the fact that Google scrapes billions of web pages and
doesn't ask permission before making the cache copies available. This
scraping is used as a carrier for the ads that make Google stinkin' rich.
Now that, in our opinion, is an interesting copyright issue. As this is
written, Google has a market cap of $55 billion. This exceeds the market
cap of General Motors and Ford combined. Google is probably the single
largest information resource on the planet, and they're getting rich off
of us. It's time for Google to give something back to the public sector.

   We claim that "fair use" is involved when a nonprofit scrapes Google.
We do it for privacy reasons, because Google has been utterly unresponsive
to privacy issues. Our activity is completely unobtrusive to Google's
operations. We even spread our tiny load across more than a dozen of
Google's data centers.

   Do not use this program if you have the intention of making money from
it. Do not show ads on your site if you use this program on your site.
By showing ads, you hand Google a legal argument for issuing a cease and
desist against your scraping. By separating your scraping from anything
that provides income or profit, you stand a better chance of prevailing
in court under a "fair use" argument. At PIR we think it would be splendid
if scraping Google for nonprofit purposes, and stripping out their wretched
advertising, was established someday as an acceptable, legal practice.
That's our opinion, for what it's worth.

-- Daniel Brandt, PIR president and programmer

____________  start of program:

Runs on Linux, compiles with gcc.

NOTE: unsigned char is the typecast used for all "char" in this program,
which allows the use of ASCII range 160-255 that is defined in ISO 8859-1.

Change history:

2005-01-11: Some have requested a "bookmark" or "shortcut" capability, so
            two doorway pages are now offered. One uses GET and one uses
            the original POST. This program was modified to first check
            for valid search terms in a QUERY_STRING. If that fails,
            then the readpost() function is called to check for valid
            terms using the POST method.
2005-01-12: After perusing the Firefox search plugin literature, which
            uses the GET method, two alterations were made to expand
            this method in this proxy. a) Either Gw= or q= is okay
            after the question mark in this program (the latter is
            typically used by Google); and b) the QUERY_STRING is terminated
            on the first '&' to truncate anything inserted by a plugin that
            we cannot use, and would cause the input string to fail our
            tests if included.
2005-01-13: A test-mode option for running the program from the command
            line instead of through Apache is available by uncommenting
            six lines in the main() function. The search terms are passed
            to the program as an argument, and the fork to curl is done
            to fetch the results from Google. Note that your directory
            paths will be different than ours, and this test mode is no
            substitute for first studying this program to determine what
            changes may be required to make it work on your system.
2005-01-18: Check-box options were added to let the searcher select 20,
            50, or 100 results. The box for 20 is checked by default on
            the cartoon entry page, the one for 50 is checked on the
            no-cartoon page, and Firefox plugins default to 100. This
            reduces bandwidth.
2005-01-24: Fixed errors in spacing and font size in newform()
2005-01-24: Code for handling a search box on a webmaster's site was added.
            This code detects whether it is a site-only search or a full
            web search, as indicated with a check-box on that site. While
            our sample paste-in code for the webmaster indicates a POST
            method, the program was amended for both GET and POST. The form
            for webmasters is at http://www.scroogle.org/masters.html
2005-01-25: We commented out the two curl forks -- one line in forkit() and
            one line in fixgoo() -- and are experimenting with snarf. You can
            download version 7.0 at http://www.xach.com/snarf/  It seems to
            be as fast or faster than curl, and the memory footprint doesn't
            even break 0.0 in our process table (curl is 0.3). The default
            "snarf" user-agent gets blocked by Google, but there's a "-m"
            switch and a "-z" switch to choose different agents. You can
            also change all three agents at the top of http.c and recompile.
            If you think it might run on your Linux, our binary is at
            http://www.scroogle.org/zipdir/snarf.zip  It's tiny by today's
            standards, because we changed the Makefile to compile with
            gcc -O2 -s -static and used a 1998 vintage Linux. It weighs
            in at a mere 140,136 bytes after you unzip it.
2005-01-26: A second form was added to the top of the results page at the
            start of parselines(), in addition to the form at the bottom.
2005-01-27: There's a list of 33 IP addresses that work as of today in
            forkit(). In fixgoo() a DNS lookup is no longer used. Instead
            the timesecs value is shifted, which means a different IP
            address will be selected from the goochkip file. After this
            change, fixgoo() became identical to forkit(), so it was
            deleted as redundant and forkit() is used both times.
2005-01-28: The modulus calculation in forkit() is now based on the
            process ID, rather than on the current second (which was
            called "timesecs"). This provides much-improved randomization
            of the selected IP address. And main() was getting bloated,
            so some of its code was moved to new functions, reservemem()
            and userinput().
2005-02-02: The list of working IP addresses in forkit() was expanded.
2005-03-02: Support was added for returning pages written in one of ten
            West European languages. Each language uses its own tiny static
            doorway page to set the language. When the &l=?? is detected
            in our input, this parameter is included in subsequent forms
            as a hidden variable.
            XX     preset default condition; our &l= is missing so do
                   nothing at all. The Google string goes out as &lr=[blank]
                   as it normally does, which returns any language
            da     Danish
            nl     Dutch
            fi     Finnish
            fr     French
            de     German
            it     Italian
            no     Norwegian
            pt     Portuguese
            es     Spanish
            sv     Swedish
            These are supported by ISO-8859-1.
2005-03-30: Changes were made in the comments under forkit() regarding the
            list of Google IP addresses.
2005-04-24: This program now presumes the UTF-8 character set instead of
            ISO-8859-1. Support was expanded to 28 languages.
2005-04-26: Some browsers return Unicode decimal entities from form inputs
            instead of UTF-8 when the character is non-ASCII; this happens
            with GET requests but usually not with POST requests. The search
            works, but the input box doesn't show the terms the way the
            searcher expects. A Unicode-decimal-to-UTF-8 encoder was added
            to translate search terms that arrive in decimal format.
2005-10-20: For the first time in over three years, Google changed the
            format of the files returned by their /ie interface. This
            required changes in getwget(), parselines() and newform().
2005-10-29: changed link color from magenta to green

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>                  /* needed for ftable in repeater() */
#include <sys/wait.h>                             /* needed for waitpid() */
#include <sys/types.h>                            /* needed for waitpid() */
#include <signal.h>                        /* needed to kill() hung child */

struct linearray
{
   unsigned char s[ 3001 ];
};
struct linearray *arl;                            /* 3001 * 102 = 306,102 */

struct tarray
{
   unsigned char s[ 299 ];
};
struct tarray *term;       /* highlight up to 4 search terms (5th = null) */

unsigned char *bufin;               /* two 8192 buffers for heavy lifting */
unsigned char *bufout;
unsigned char *co1;                         /* another two for colorizing */
unsigned char *co2;
unsigned char *genv1;
unsigned char *genv2;
unsigned char *genv3;
unsigned char *genv5;
unsigned char *genv90;
unsigned char *genv91;
unsigned char *input;
unsigned char *nu;

int denied, toparl, oldsearch;

unsigned char linebuf[ 753 ], resource[ 768 ], title[ 453 ], hijacker[ 81 ];
unsigned char linebuf2[ 753 ], lineone[ 31 ], logbuf[ 250 ], site[ 81 ];
unsigned char savedomain[ 81 ];    /* abuser() sets this from environment */
unsigned char savequad[ 20 ];      /* abuser() sets this from environment */
unsigned char shortquad[ 20 ];     /* abuser() sets this from environment */
unsigned char cr[] = { "\n" };   /* newline char for string concatenation */
unsigned char wdump[ 40 ];           /* curl dumps to this temporary file */
unsigned char errorterm[ 90 ];            /* info on errorout() condition */
unsigned char magurl[ 1203 ];      /* extract magenta url line from bufin */
unsigned char resnum[] = { "=100" };    /* checkbox option (=20,=50,=100) */
unsigned char lang[] = { "XX" };                  /* language for results */
unsigned char langtest[] = { "/ar/zs/zt/cs/da/nl/en/et/fi/fr/de/el/iw/hu/is/it/ja/ko/lv/lt/no/pt/pl/ro/ru/es/sv/tr/" };
unsigned char langtestsub[] = { "/XX/" };
unsigned char rawutf[ 5 ];                      /* returns UTF-8 encoding */
unsigned char bigbuf[ 100000 ];

time_t t2log;
pid_t procid;

void reservemem( void );
int userinput( void );
void abuser( void );
void readpost( void );
void monitor( void );
void decode( int mode );
void encode( void );
void nblogging( void );
void errorout( void );
void noresults( void );
void parselines( void );
void stripper( void );
void getwget( void );
void newform( void );
void drivecolor( void );
void getterms( char *allterm );
void colorize( int ele );
void forkit( void );
void loadcheck( void );
void concaturl( void );
void overload( void );
void repeater( void );
void numtobin( int c );
void driveutf8( void );

/* ________________________________________________________ FUNCTION MAIN */

int main( int argc, char *argv[] )
{

   reservemem();
   abuser();               /* gets domains, checks for locked out domains */
   loadcheck();                        /* checks for heavy load on server */

//  For testing purposes, the following six lines can be uncommented and the
//  program run from the command line by adding a search term as an argument.
//       For example:   nbbw.cgi Gw=scroogle+scraper >test.html
//  If the required external files are set up properly, you should find the
//  scraped Google results for the term "scroogle scraper" in test.html.
//  Line one will show Content-type: text/html, line two is blank, and the
//  remainder of the file is the scraped results as shown to the end user.
//  Once this works, all that remains is to get Apache to accept it. Make
//  a test html page with a form on it and try to execute it through the web.
// 
//   if( argc == 2 )
//   {
//      strcpy( bufout, "QUERY_STRING=" );
//      strcat( bufout, argv[ 1 ] );
//      putenv( bufout );
//   }

   bufin[ 0 ] = '\0';
   if( ( genv3 = getenv( "QUERY_STRING" ) ) != NULL )
      strncpy( bufin, genv3, 8190 );

   if( userinput() )
      readpost();                   /* QUERY_STRING failed; try POST next */

   bufin[ 0 ] = '\0';
   decode( 1 );                           /* double decode is recommended */
   decode( 2 );
   stripper();                                /* take out hacker mischief */
   driveutf8();                   /* Unicode decimal to UTF-8 translation */
   encode();                              /* returns with resource string */
   repeater();               /* has this exact search been done recently? */
   monitor();                                  /* check for > 20 per hour */
   if( !oldsearch )
      forkit();
   getwget();
   parselines();                                /* parse and output array */
   nblogging();
   exit( 0 );

} /* ___________________________________________________________ END MAIN */


/* __________________________________________________ FUNCTION RESERVEMEM */

void reservemem( void )
{

   bufin = (unsigned char *) malloc( (size_t)sizeof( unsigned char ) * 8192 );
   bufout = (unsigned char *) malloc( (size_t)sizeof( unsigned char ) * 8192 );
   co1 = (unsigned char *) malloc( (size_t)sizeof( unsigned char ) * 8192 );
   co2 = (unsigned char *) malloc( (size_t)sizeof( unsigned char ) * 8192 );
   term = (struct tarray *) malloc( (size_t)sizeof( struct tarray ) * 5 );
   arl = (struct linearray *) malloc( (size_t)sizeof( struct linearray ) * 102 );

   if( arl == NULL )
   {
      strcpy( errorterm, "malloc failure" );
      errorout();
   }

   time( &t2log );
   strncpy( logbuf, ctime( &t2log ) + 4, 13 );
   logbuf[ 12 ] = ' ';
   logbuf[ 13 ] = '\0';
   procid = getpid();     /* for temp filename, and also used in forkit() */
   sprintf( wdump, "/home/slops/scslop/wgn%d", procid );

} /* _____________________________________________________ END RESERVEMEM */


/* ___________________________________________________ FUNCTION USERINPUT */

int userinput( void )
{
   int w, badget = 0;

   if( bufin[ 0 ] )                                            /* test #1 */
   {
      if( !strncmp( bufin, "Gw=", 3 ) )                   /* "Gw=" exists */
      {
         nu = strstr( bufin, "&n=" );                  /*  look for "&n=" */
         if( nu != NULL )
            resnum[ 1 ] = *( nu + 3 );
         if( resnum[ 1 ] == '2' || resnum[ 1 ] == '5' )
            resnum[ 3 ] = '\0';
         nu = strstr( bufin, "&d=" );        /* change history 2005-01-24 */
         if( nu != NULL )          /* search box came from someone's site */
         {
                    /* first letter of a valid domain wouldn't be encoded */
            if( *( nu + 3 ) != '*' && *( nu + 3 ) != '%' )
            {
               strcpy( site, "site:" );
               strncat( site, nu + 3, 70 );
               w = strcspn( site, "&= +" );     /* bad chars for a domain */
               site[ w ] = '\0';      /* terminate on first bad character */
            }
         }
      }
      else
      {
         if( strncmp( bufin, "q=", 2 ) )     /* change history 2005-01-12 */
            badget = 1;
      }
   }
   else
      badget = 1;                                       /* bufin is empty */

   if( !badget )                                               /* test #2 */
   {
      nu = strstr( bufin, "&l=" );                     /*  look for "&l=" */
      if( nu )
      {
         langtestsub[ 1 ] = *( nu + 3 );
         langtestsub[ 2 ] = *( nu + 4 );
         if( strstr( langtest, langtestsub ) != NULL )
         {
            lang[ 0 ] = *( nu + 3 );
            lang[ 1 ] = *( nu + 4 );
         }
         else
         {
            lang[ 0 ] = 'X';
            lang[ 1 ] = 'X';
         }
      }
      nu = strchr( bufin, '&' );
      if( nu )
         *( nu ) = '\0';                        /* terminate on first '&' */
      if( bufin[ 0 ] == 'G' )
         strncpy( linebuf, bufin + 3, 405 );      /* strip Gw= from front */
      else if( bufin[ 0 ] == 'q' )
         strncpy( linebuf, bufin + 2, 405 );       /* strip q= from front */
      linebuf[ 405 ] = '\0';
      if( site[ 0 ] )
      {
         strcat( linebuf, " " );
         strcat( linebuf, site );
         linebuf[ 405 ] = '\0';
      }
      if( strlen( linebuf ) < 3 )
         badget = 1;
   }
   if( badget )
      return( 1 );
   else
      return( 0 );

} /* ______________________________________________________ END USERINPUT */


/* ______________________________________________________ FUNCTION ABUSER */

void abuser( void )
{   
   int hit = 0, x, y = 0, f = 0, len;
   unsigned char logabu[ 81 ];
   static FILE *file90;
   static FILE *file94;

      /* If the referrer is in /gcslop/refdeny, we exit immediately with
            a "Forbidden." This is in case of a major hijacking incident. */

   hijacker[ 0 ] = '\0';
   if( ( genv5 = getenv( "HTTP_REFERER" ) ) != NULL )
   {
      strncpy( hijacker, genv5, 78 );
      hijacker[ 78 ] = '\0';
   }
   if( !strncmp( hijacker, "http://www.scroogle.org", 23 ) )
      hijacker[ 0 ] = '\0';
   else if( !strncmp( hijacker, "http://scroogle.org", 19 ) )
      hijacker[ 0 ] = '\0';
   if( strlen( hijacker ) > 7 )
   {
      if( ( file94 = fopen( "/home/slops/scslop/refdeny", "rb" ) ) != NULL )
      {
         fgets( logabu, 79, file94 );
         while( !feof( file94 ) )
         {
            len = strlen( logabu );
            if( len > 2 )
            {
               logabu[ len - 1 ] = '\0';
               if( strstr( hijacker, logabu ) != NULL )
               {
                  fclose( file94 );
                  fputs( "Status: 403 Forbidden\n", stdout );
                  fputs( "Content-type: text/html; charset=UTF-8\n\n", stdout );
                  fflush( stdout );
                  fputs( "<HTML><HEAD><META HTTP-EQUIV=\"content-type\"  CONTENT=\"text/html; charset=UTF-8\">\n", stdout );
                  fputs( "<TITLE> </TITLE></HEAD><BODY><H1>Forbidden</H1>You don't have permission to access /cgi-bin/ on this server.</BODY></HTML>\n", stdout );
  /* cannot log here because we don't have our savedomain or savequad yet */
                  exit( 0 );
               }
            }
            fgets( logabu, 79, file94 );
         }
         fclose( file94 );
      }
   }
   else
      hijacker[ 0 ] = '\0';

   savequad[ 0 ] = '\0';
   bufin[ 0 ] = '\0';
   if( ( genv91 = getenv( "REMOTE_ADDR" ) ) != NULL )
      strcpy( bufin, genv91 );
   if( bufin[ 0 ] )
   {
      bufin[ 19 ] = '\0';
      strcpy( savequad, bufin );
      bufin[ 0 ] = '\0';
   }
   if( ( genv90 = getenv( "REMOTE_HOST" ) ) != NULL )
      strcpy( bufin, genv90 );
   if( bufin[ 0 ] )
   {
      bufin[ 79 ] = '\0';
      strcpy( savedomain, bufin );
   }
   else
   {
      if( savequad[ 0 ] )
         strcpy( savedomain, savequad );
      else
         strcpy( savedomain, "remote not found" );
   }
   if( savequad[ 0 ] )
   {
      strcpy( shortquad, savequad );
      for( x = 1; shortquad[ x ]; x++ )
      {
         if( shortquad[ x ] == '.' )
         {
            y++;
            if( y == 3 )
               shortquad[ x + 1 ] = '\0';
         }
      }
   }
   else
      return;

       /* If the Class C is in /scslop/abuser, we exit with a "Too busy."
            If there is an asterisk after it, we exit with a "Forbidden." */

   strcat( shortquad, "\n" );                 /* shortquad is the Class C */
   if( ( file90 = fopen( "/home/slops/scslop/abuser", "rb" ) ) == NULL )
      return;
   fgets( logabu, 79, file90 );
   while( !feof( file90 ) )
   {
      if( logabu[ strlen( logabu ) - 2 ] == '*' )
      {
         f = 1;
         logabu[ strlen( logabu ) - 1 ] = '\0';
      }
      else
         f = 0;
      x = strlen( logabu );
      logabu[ x - 1 ] = '\0';                      /* strip off \n or '*' */
      if( logabu[ x - 2 ] == '.' )
      {
         if( strstr( shortquad, logabu ) != NULL )
            hit = 1;
      }
      else
      {
         if( strstr( savedomain, logabu ) != NULL )
            hit = 1;
      }
      fgets( logabu, 79, file90 );
   }  
   fclose( file90 );

   if( hit )
   {
      if( f )
      {
         fputs( "Status: 403 Forbidden\n", stdout );
         fputs( "Content-type: text/html; charset=UTF-8\n\n", stdout );
         fflush( stdout );
         fputs( "<HTML><HEAD><META HTTP-EQUIV=\"content-type\"  CONTENT=\"text/html; charset=UTF-8\">\n", stdout );
         fputs( "<TITLE> </TITLE></HEAD><BODY><H1>Forbidden</H1>You don't have permission to access /cgi-bin/ on this server.</BODY></HTML>\n", stdout );
      }
      else
      {
         fputs( "Content-type: text/plain\n\n", stdout );
         fflush( stdout );
         fputs( "Server too busy.\n\n", stdout );
      }
      exit( 0 );
   }

} /* _________________________________________________________ END ABUSER */


/* _____________________________________________________ FUNCTION MONITOR */

void monitor( void )
{   
                           /* the monitor file is emptied once an hour by
                                 a different program that runs under cron */
   int cnt = 0, thresh;
   static FILE *file92;
   static FILE *file93;

   strcpy( bufout, shortquad );
   bufout[ strlen( bufout ) - 1 ] = '\0';
   if( bufout[ 0 ] )
      strcat( bufout, "F\n" );
   else
      strcpy( bufout, "goo.local.F\n" );
   if( ( file92 = fopen( "/home/slops/nbslop2/monitor", "rb" ) ) == NULL )
      return;
   fgets( linebuf, 79, file92 );
   while( !feof( file92 ) )
   {
      if( !strcmp( bufout, linebuf ) )
         cnt++;
      fgets( linebuf, 79, file92 );
   }  
   fclose( file92 );

         /* if they repeat searches I try to give them a break on the usual
            threshold, but I still have to count any oldsearch in case of a
            runaway browser that keeps looping on the same search */
   if( !oldsearch )
      thresh = 19;
   else
      thresh = 25;

   if( cnt > thresh )
   {
      fputs( "Content-type: text/html; charset=UTF-8\n\n", stdout );
      fflush( stdout );
      fputs( "<HTML><HEAD><META HTTP-EQUIV=\"content-type\"  CONTENT=\"text/html; charset=UTF-8\">\n", stdout );
      fputs( "<TITLE>Scroogle Proxy</TITLE></HEAD><BODY><BR><BR><blockquote>\n", stdout );
      fputs( "<b>Sorry, but we have a limit of 20 searches per hour per user due to our limited bandwidth.</b>\n", stdout );
      fputs( "\n<P></blockquote>\n</BODY>\n</HTML>\n", stdout );
      denied = 1;
      nblogging();
      exit( 0 );
   }
   else
   {
      if( ( file93 = fopen( "/home/slops/nbslop2/monitor", "ab" ) ) == NULL )
         return;
      fputs( bufout, file93 );
      fclose( file93 );
   }

} /* ________________________________________________________ END MONITOR */


/* ____________________________________________________ FUNCTION READPOST */

void readpost( void )
{
   int x, conlen = 0;

   if( ( genv1 = getenv( "REQUEST_METHOD" ) ) == NULL )
   {
      strcpy( errorterm, "no REQUEST_METHOD" );
      errorout();
   }
   strcpy( bufin, genv1 );
   if( strcmp( bufin, "POST" ) )
   {
      strcpy( errorterm, "no POST" );
      errorout();
   }
   if( ( genv2 = getenv( "CONTENT_LENGTH" ) ) == NULL )
   {
      strcpy( errorterm, "no CONTENT_LENGTH" );
      errorout();
   }
   conlen = atoi( genv2 );
   if( conlen < 6 || conlen > 420 )
   {
      sprintf( errorterm, "conlen=%d", conlen );
      errorout();
   }
   input = (unsigned char *) malloc( (size_t)sizeof( unsigned char ) * conlen + 1 );
   x = fread( input, (size_t)sizeof( unsigned char ), conlen, stdin );
   if( x != conlen )
   {
      sprintf( errorterm, "x=%d conlen=%d", x, conlen );
      errorout();
   }
   else
   {
      input[ conlen ] = '\0';
      if( strncmp( input, "Gw=", 3 ) )                 /* no Gw= in front */
      {
         strcpy( errorterm, "no Gw=" );
         errorout();
      }
      nu = strstr( input, "&n=" );
      if( nu != NULL )
      {
         resnum[ 1 ] = *( nu + 3 );
         if( resnum[ 1 ] == '2' || resnum[ 1 ] == '5' )
            resnum[ 3 ] = '\0';
      }
      nu = strstr( input, "&d=" );           /* change history 2005-01-24 */
      if( nu != NULL )             /* search box came from someone's site */
      {
                    /* first letter of a valid domain wouldn't be encoded */
         if( *( nu + 3 ) != '*' && *( nu + 3 ) != '%' )
         {
            strcpy( site, "site:" );
            strncat( site, nu + 3, 70 );
            x = strcspn( site, "&= +" );        /* bad chars for a domain */
            site[ x ] = '\0';         /* terminate on first bad character */
         }
      }
      nu = strstr( input, "&l=" );                     /*  look for "&l=" */
      if( nu )
      {
         langtestsub[ 1 ] = *( nu + 3 );
         langtestsub[ 2 ] = *( nu + 4 );
         if( strstr( langtest, langtestsub ) != NULL )
         {
            lang[ 0 ] = *( nu + 3 );
            lang[ 1 ] = *( nu + 4 );
         }
         else
         {
            lang[ 0 ] = 'X';
            lang[ 1 ] = 'X';
         }
      }
      nu = strchr( input, '&' );
      if( nu )
         *( nu ) = '\0';                        /* terminate on first '&' */
      strncpy( linebuf, input + 3, 405 );         /* strip Gw= from front */
      linebuf[ 405 ] = '\0';
      if( site[ 0 ] )
      {
         strcat( linebuf, " " );
         strcat( linebuf, site );
         linebuf[ 405 ] = '\0';
      }
      if( strlen( linebuf ) < 3 )
      {
         sprintf( errorterm, "lb<3: *%s*", linebuf );
         errorout();
      }
   }

} /* _______________________________________________________ END READPOST */


/* ______________________________________________________ FUNCTION DECODE */

void decode( int mode )
{
   int x, y;
   long int z;
   unsigned char tembuf[ 753 ];
   unsigned char hexnum[] = "  ";
   char *endhex;            /* strtol() requires char, not unsigned char */
 
            /* special decoder: comes in linebuf and goes out in linebuf */
   y = 0;
   for( x = 0; linebuf[ x ]; x++ )
   {
      if( linebuf[ x ] == '%' )
      {
         hexnum[ 0 ] = linebuf[ x + 1 ];
         hexnum[ 1 ] = linebuf[ x + 2 ];
         z = strtol( hexnum, &endhex, 16 );
         tembuf[ y ] = (unsigned char)z;
         y++;
         x += 2;
      }
      else
      {
         tembuf[ y ] = linebuf[ x ];
         if( tembuf[ y ] == '+' && mode == 1 )
            tembuf[ y ] = ' ';
         y++;
      }
   }
   tembuf[ y ] = '\0';
   strcpy( linebuf, tembuf );

} /* _________________________________________________________ END DECODE */


/* ____________________________________________________ FUNCTION STRIPPER */

void stripper( void )
{
                       /* Input arrives in linebuf and leaves in linebuf. */
   int x, y, z;
   static char ok_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\"@#_-. :',?/~&;+";
   unsigned char linebuf1[ 753 ];
   unsigned char *cp1;

   for( x = 0; linebuf[ x ]; x++ )
   {
      if( (int)linebuf[ x ] > 127 )            /* accept UTF-8 characters */
         continue;
      cp1 = strchr( ok_chars, linebuf[ x ] );
      if( cp1 == NULL )
         linebuf[ x ] = ' ';
   }
   strcpy( linebuf1, linebuf );                         /* copy to buffer */

   y = 0;                                   /* close up contiguous spaces */
   z = 0;
   for( x = 0; linebuf1[ x ]; x++ )
   {
      if( linebuf1[ x ] == ' ' )
      {
         if( z )
            continue;
         z = 1;
      }
      else
         z = 0;
      linebuf[ y ] = linebuf1[ x ];
      y++;
   }
   linebuf[ y ] = '\0';
   if( linebuf[ y - 1 ] == ' ' )
   {
      y--;
      linebuf[ y ] = '\0';
   }
   if( linebuf[ 0 ] == ' ' )
   {
      strcpy( linebuf1, linebuf + 1 );
      strcpy( linebuf, linebuf1 );
      y--;
   }
   if( y < 3 )
   {
      sprintf( errorterm, "y<3: *%s*", linebuf );
      errorout();
   }

} /* _______________________________________________________ END STRIPPER */


/* ______________________________________________________ FUNCTION ENCODE */

void encode( void )
{
   int x, z;
   unsigned char temp1[] = { "%" };
   unsigned char temp2[ 10 ];

   z = 0;
   for( x = 0; linebuf[ x ] != '\0'; x++ )
   {
      if( !isalnum( linebuf[ x ] ) )
      {
         if( linebuf[ x ] == ' ' )
         {
            linebuf2[ z ] = '+';
            z++;
            continue;
         }
         sprintf( temp2, "%X", linebuf[ x ] );
         linebuf2[ z ] = '\0';
         strcat( linebuf2, temp1 );
         strcat( linebuf2, temp2 );
         z += 3;
         continue;
      }
      linebuf2[ z ] = linebuf[ x ];
      z++;
   }
   linebuf2[ z ] = '\0';        /* QUERY_STRING keyword segment now ready */


} /* _________________________________________________________ END ENCODE */


/* ___________________________________________________ FUNCTION NBLOGGING */

void nblogging( void )
{
   static FILE *file10;

   strcat( logbuf, savedomain );
   if( !denied || denied == 3 )
      strcat( logbuf, "  G s p  " );
   else if( denied == 1 )
      strcat( logbuf, "  G s denied  " );
   else if( denied == 2 )
   {
      strcat( logbuf, "  G s e  " );
      strcat( logbuf, errorterm );
      strcat( logbuf, "  " );
   }
   if( hijacker[ 0 ] )
   {
      strcat( logbuf, "hijack:" );
      strcat( logbuf, hijacker );
      strcat( logbuf, "  " );
   }
   if( strcmp( savedomain, savequad ) )
      strcat( logbuf, savequad );
   if( lineone[ 0 ] )
   {
      strcat( logbuf, "  " );
      strcat( logbuf, lineone );
   }
   strcat( logbuf, cr );

   if( strlen( logbuf ) > 148 )
   {
      logbuf[ 147 ] = '\n';
      logbuf[ 148 ] = '\0';
   }

   if( ( file10 = fopen( "/home/slops/nbslop2/nblog3", "ab" ) ) == NULL )
      return;
   fputs( logbuf, file10 );
   fclose( file10 );
   logbuf[ 13 ] = '\0';                  /* leave it ready for future use */

} /*_______________________________________________________ END NBLOGGING */


/* _____________________________________________________ FUNCTION GETWGET */

void getwget( void )
{
   unsigned char *cp1, *cp2;
   static FILE *file31;
   static FILE *file85;

   if( ( file31 = fopen( wdump, "rb" ) ) == NULL )
   {
      strcpy( errorterm, "fixgoo" );
      denied = 2;
      nblogging();
      denied = 0;
      procid += 10;                    /* don't use the same IP this time */
      forkit();                        /* file missing! try one more time */
      if( ( file31 = fopen( wdump, "rb" ) ) == NULL )
      {
         if( ( file85 = fopen( "/home/slops/scslop/terms", "ab" ) ) != NULL )
         {
            sprintf( linebuf, "%s 000  timeout  %s\n", logbuf, lineone );
            fputs( linebuf, file85 );
            fclose( file85 );
         }
         strcat( errorterm, "  wdump not found" );
         errorout();
      }
   }
   fgets( bufout, 8190, file31 );
   cp1 = strstr( bufout, "<title>" );
   if( cp1 != NULL )
   {
      strncpy( title, cp1 + 7, 451 );
      title[ 451 ] = '\0';
      cp2 = strstr( title, "</title>" );
      if( cp2 != NULL )
         *( cp2 ) = '\0';
   }
   else
   {
      strcpy( errorterm, "no title" );
      errorout();
   }
   fgets( bufout, 8190, file31 );                       /* throwaway line */
   fgets( bigbuf, 99997, file31 );                           /* mega line */
   fclose( file31 );

   cp1 = strstr( bigbuf, "<nobr>" );
   while( cp1 != NULL )
   {
      cp1 = cp1 + 6;
      cp2 = strstr( cp1, "<br></nobr>" );
      if( cp2 == NULL )
         break;
      *( cp2 ) = '\0';
      strncpy( arl[ toparl ].s, cp1, 2999 );
      arl[ toparl ].s[ 2997 ] = '\n';
      arl[ toparl ].s[ 2998 ] = '\0';
      if( toparl < 101 )
         toparl++;
      cp2 = cp2 + 11;
      cp1 = strstr( cp2, "<nobr>" );
   }

   /* Normally we might remove( wdump ) here, but in this program the
      wdump files are removed once per hour by an external program
      running on a cron job that does a system( "rm /scslop/wg*" ). */

   if( !toparl )
      noresults();

} /* ________________________________________________________ END GETWGET */


/* ____________________________________________________ FUNCTION ERROROUT */

void errorout( void )
{

   fputs( "Content-type: text/html; charset=UTF-8\n\n", stdout );
   fflush( stdout );
   fputs( "<HTML><HEAD><META HTTP-EQUIV=\"content-type\"  CONTENT=\"text/html; charset=UTF-8\">\n", stdout );
   fputs( "<META NAME=\"ROBOTS\" CONTENT=\"NOINDEX,NOFOLLOW\">\n", stdout );
   fputs( "<TITLE>Scroogle Proxy</TITLE></HEAD><BODY>\n", stdout );
   fputs( "<font face=\"Arial, Helvetica, sans-serif\"><BR><BR>\n", stdout );
   fputs( "<CENTER><H3>Sorry, but we are unable to provide results from Google.</H3></CENTER>\n", stdout );
   fputs( "<center><font size=2>Most errors are because three or more\n", stdout );
   fputs( "valid characters must be in the<BR>search box before we pass them along.\n", stdout );
   fputs( "Languages other than English<BR>require that your browser support the\n", stdout );
   fputs( "UTF-8 character set. Sometimes<BR>our request to Google does not get\n", stdout );
   fputs( "through, and you should try again.</center><br><CENTER>\n", stdout );
   fputs( "<A Href=\"http://www.scroogle.org/cgi-bin/scraper.htm\">Back to search page</A>\n", stdout );
   fputs( "</CENTER><P></font>\n</BODY>\n</HTML>\n", stdout );

   if( !denied )                      /* if error not otherwise specified */
   {
      denied = 2;
      if( !errorterm[ 0 ] )
         strcpy( errorterm, "**" );
   }
   nblogging();
   exit( 0 );

} /* _______________________________________________________ END ERROROUT */


/* ___________________________________________________ FUNCTION NORESULTS */

void noresults( void )
{
   static FILE *file86;

   if( !title[ 0 ] )
      strcpy( title, "Scroogle Proxy" );
   fputs( "Content-type: text/html; charset=UTF-8\n\n", stdout );
   fflush( stdout );
   fputs( "<HTML><HEAD><META HTTP-EQUIV=\"content-type\"  CONTENT=\"text/html; charset=UTF-8\">\n", stdout );
   printf( "<META NAME=\"ROBOTS\" CONTENT=\"NOINDEX,NOFOLLOW\">\n<TITLE>%s</TITLE>\n</HEAD>\n<BODY>\n", title );
   fputs( "<font face=\"Arial, Helvetica, sans-serif\"><BR><blockquote><BR>\n", stdout );
   printf( "<b>%s</b><P>", title );
   fputs( "<b>Google returned no results for this search.</b>\n", stdout );
   newform();
   fputs( "\n<P>\n<A Href=\"http://www.scroogle.org/cgi-bin/scraper.htm\">Back to search page</A>", stdout );
   fputs( "\n<P></blockquote>\n</BODY>\n</HTML>\n", stdout );

   if( ( file86 = fopen( "/home/slops/scslop/terms", "ab" ) ) != NULL )
   {
      strcpy( linebuf, logbuf );
      strcat( linebuf, " 000" );
      if( lineone[ 0 ] )
      {
         strcat( linebuf, "  " );
         strcat( linebuf, lineone );
      }
      strcat( linebuf, cr );
      fputs( linebuf, file86 );
      fclose( file86 );
   }
   denied = 3;
   nblogging();
   exit( 0 );

} /* ______________________________________________________ END NORESULTS */


/* __________________________________________________ FUNCTION PARSELINES */

void parselines( void )
{
   int q;
   unsigned char *cp1;
   unsigned char title2[ 453 ];

   /* bufin will be the number and the anchor; bufout will be the snippet;
      neither bufin nor bufout will be terminated after they are stuffed  */

   fputs( "Content-type: text/html; charset=UTF-8\n\n", stdout );
   fflush( stdout );
   if( !title[ 0 ] )
      strcpy( title, "Scroogle Proxy" );
   fputs( "<HTML><HEAD><META HTTP-EQUIV=\"content-type\"  CONTENT=\"text/html; charset=UTF-8\">\n", stdout );
   fputs( "<META NAME=\"ROBOTS\" CONTENT=\"NOINDEX,NOFOLLOW\">\n", stdout );
   printf( "<TITLE>%s</TITLE>\n</HEAD>\n<BODY BGCOLOR=#ffffff>\n", title );
   fputs( "<BR><font face=\"Arial, Helvetica, sans-serif\"><blockquote>\n", stdout );
   strcpy( title2, title );
   cp1 = strstr( title2, " - Google Search" );
   if( cp1 != NULL )
   {
      *( cp1 ) = '\0';
      fputs( "<center><FORM METHOD=POST ACTION=\"http://www.scroogle.org/cgi-bin/nbbw.cgi\">\n", stdout );
      printf( "<INPUT TYPE=TEXT NAME=\"Gw\" VALUE=\"%s\" SIZE=\"30\" MAXLENGTH=\"225\">\n", title2 );
      fputs( "&nbsp;&nbsp;<INPUT TYPE=\"submit\" VALUE=\"Search\">\n", stdout );
      if( resnum[ 1 ] == '2' )
         fputs( "<INPUT TYPE=\"hidden\" NAME=\"n\" VALUE=\"2\">\n", stdout );
      else if( resnum[ 1 ] == '5' )
         fputs( "<INPUT TYPE=\"hidden\" NAME=\"n\" VALUE=\"5\">\n", stdout );
      else if( resnum[ 1 ] == '1' )
         fputs( "<INPUT TYPE=\"hidden\" NAME=\"n\" VALUE=\"1\">\n", stdout );
      if( lang[ 0 ] != 'X' )
         printf( "<INPUT TYPE=\"hidden\" NAME=\"l\" VALUE=\"%s\">\n", lang );
      fputs( "</FORM></center>\n", stdout );
      getterms( title2 );
   }
   printf( "<b>%s</b><br><br>\n", title );

   for( q = 0; q < toparl; q++ )
   {
                      /* if the line had to be truncated, it's not usable */
      if( strlen( arl[ q ].s ) > 2990 )
         continue;
      strcpy( bufin, arl[ q ].s );
      cp1 = strstr( bufin, "<a title=" );
      if( cp1 == NULL )
         continue;
      *( cp1 ) = '\0';               /* this saves the leading hit number */
      strcpy( bufout, cp1 + 9 );          /* get snippet; starts with '"' */
      if( bufout[ 0 ] != '"' )
         continue;
      cp1 = strchr( bufout + 1, '"' );  /* supposed to end with quote too */
      if( cp1 == NULL )
         continue;
      *( cp1 + 1 ) = '\0';
      cp1 = strstr( arl[ q ].s, "href=" );
      if( cp1 == NULL )
         continue;
      strcat( bufin, "<A Href=\"" );
      strcat( bufin, cp1 + 5 );
      cp1 = strchr( bufin, '>' );
      if( cp1 == NULL )
         continue;
      *( cp1 ) = '"';        /* we've changed > to " and terminated bufin */
      *( cp1 + 1 ) = '\0';
      cp1 = strchr( arl[ q ].s, '>' );   /* now backtrack; we lost a char */
      strcat( bufin, cp1 );
      drivecolor();
      concaturl();
      printf( "%s\n<ul><font size=2>%s%s</font></ul>\n", bufin, bufout, magurl );
   }

   newform();

   fputs( "\n<P></blockquote>\n</BODY>\n</HTML>\n", stdout );

} /* _____________________________________________________ END PARSELINES */


/* _____________________________________________________ FUNCTION NEWFORM */

void newform( void )
{
                                   /* provides a new form with values set */
   unsigned char *instr3;
   unsigned char title2[ 453 ];

   strcpy( title2, title );
   instr3 = strstr( title2, " - Google Search" );
   if( instr3 == NULL )
      return;
   *( instr3 ) = '\0';
   fputs( "<BR><BR><center>\n<FORM METHOD=POST ACTION=\"http://www.scroogle.org/cgi-bin/nbbw.cgi\">\n", stdout );
   printf( "<INPUT TYPE=TEXT NAME=\"Gw\" VALUE=\"%s\" SIZE=\"40\" MAXLENGTH=\"225\">\n", title2 );
   fputs( "&nbsp;&nbsp;<INPUT TYPE=\"submit\" VALUE=\"Search\">\n", stdout );
   fputs( "<BR><font size=2>number of results<b>:</b> &nbsp;\n", stdout );

   if( resnum[ 1 ] == '2' )
   {
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"2\" CHECKED>20 &nbsp; &nbsp;\n", stdout );
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"5\">50 &nbsp; &nbsp;\n", stdout );
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"1\">100\n", stdout );
   }
   else if( resnum[ 1 ] == '5' )
   {
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"2\">20 &nbsp; &nbsp;\n", stdout );
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"5\" CHECKED>50 &nbsp; &nbsp;\n", stdout );
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"1\">100\n", stdout );
   }
   else if( resnum[ 1 ] == '1' )
   {
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"2\">20 &nbsp; &nbsp;\n", stdout );
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"5\">50 &nbsp; &nbsp;\n", stdout );
      fputs( "<INPUT TYPE=RADIO NAME=\"n\" VALUE=\"1\" CHECKED>100\n", stdout );
   }
   if( lang[ 0 ] != 'X' )
      printf( "<INPUT TYPE=\"hidden\" NAME=\"l\" VALUE=\"%s\">\n", lang );
   fputs( "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <BR>\n", stdout );
   fputs( "<BR><A Href=\"http://www.scroogle.org/\">home page</A> &nbsp; &nbsp; &nbsp; &nbsp;\n", stdout );
   fputs( "<A Href=\"http://www.scroogle.org/scget.html\"><font color=#008000>how to make a bookmark</font></A>\n", stdout );
   fputs( "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; </font></FORM></center>\n", stdout );

} /* ________________________________________________________ END NEWFORM */


/* __________________________________________________ FUNCTION DRIVECOLOR */

void drivecolor( void )
{
   int f = 0;
   unsigned char *cp9;

//   printf( "\n===before============>*%s*\n", bufout );

   cp9 = strstr( bufout, "&amp;" );           /* replace all &amp; with & */
   while( cp9 != NULL )
   {
      *( cp9 + 1 ) = '\0';
      strcat( bufout, cp9 + 5 );
      cp9 = strstr( bufout, "&amp;" );
   }

                    /* send bufout to colorize( f ); it returns in bufout */
   while( term[ f ].s[ 0 ] )
   {
      colorize( f );
      f++;
   }

//   printf( "\n+++after+++++++++++++>*%s*\n", bufout );

} /* _____________________________________________________ END DRIVECOLOR */


/* ____________________________________________________ FUNCTION COLORIZE */

void colorize( int e )
{
   int r, off = 0, tt;
   unsigned char *cp7;

                 /* we use co1 to get the strstr pointer; all actions are
                  performed so as to preserve the UC/LC already in bufout
                  by concatenating bufout into co2 */

   co2[ 0 ] = '\0';
   tt = strlen( term[ e ].s );
   for( r = 0; bufout[ r ]; r++ )
      co1[ r ] = tolower( bufout[ r ] );
   co1[ r ] = '\0';
   cp7 = strstr( co1, term[ e ].s );                     /* off == 0 here */
   while( cp7 != NULL )
   {
      r = (int)( cp7 - co1 - off );           /* same as: - ( col + off ) */
      strncat( co2, bufout + off, r );
      strcat( co2, "<b style=\"background-color:#" );
      switch( e )
      {
         case 0:
            strcat( co2, "ffff66\">" );
            break;
         case 1:
            strcat( co2, "a0ffff\">" );
            break;
         case 2:
            strcat( co2, "99ff99\">" );
            break;
         case 3:
            strcat( co2, "ff9999\">" );
            break;
         default:
            return;
      }
      strncat( co2, bufout + off + r, tt );        /* add the actual term */
      strcat( co2, "</b>" );
      off = off + r + tt;
      cp7 = strstr( co1 + off, term[ e ].s );
   }
   strcat( co2, bufout + off );
   strcpy( bufout, co2 );

} /* _______________________________________________________ END COLORIZE */


/* ____________________________________________________ FUNCTION GETTERMS */

void getterms( char *allterm )
{
                            /* fill term[0-3].s with parsing from allterm */
   int x, y = 0, q = 0;
   unsigned char bf1[ 299 ], bf2[ 299 ];
   unsigned char *cp6;

   strcpy( bf1, allterm );
                                                  /* change to lower case */
   for( x = 0; bf1[ x ]; x++ )
      bf2[ x ] = tolower( bf1[ x ] );
   bf2[ x ] = '\0';

        /* strip out any "&quot;" which appear instead of quotation marks */
   cp6 = strstr( bf2, "&quot;" );
   while( cp6 != NULL )
   {
      *( cp6 + 1 ) = '\0';             /* leave just the '&' to mark spot */
      strcat( bf2, cp6 + 6 );
      cp6 = strstr( bf2, "&quot;" );
   }
   for( x = 0; bf2[ x ]; x++ )
   {
      if( bf2[ x ] == '&' )
      {
         y++;
         continue;
      }
      if( bf2[ x ] == ' ' || bf2 [ x ] == '-' )
      {
         if( y % 2 )         /* if y / 2 has remainder it's an odd number */
            bf2[ x ] = '_';
      }
   }

   y = strlen( bf2 );
   x = strcspn( bf2, " -" );            /* space or hyphen are delimiters */
   while( x < y )
   {
      bf2[ x ] = '\0';
      strcpy( term[ q ].s, bf2 );
      strcpy( bf1, bf2 + x + 1 );
      strcpy( bf2, bf1 );
      y = strlen( bf2 );
      x = strcspn( bf2, " -" );
      q++;
      if( q == 3 )                       /* term[4].s always remains null */
      {
         bf2[ x ] = '\0';
         break;
      }
   }
   strcpy( term[ q ].s, bf2 );

   for( q = 0; q < 4; q++ )
   {
      if( term[ q ].s[ 0 ] == '&' )
      {
         strcpy( bf1, term[ q ].s + 1 );
         y = strlen( bf1 );
         if( bf1[ y - 1 ] == '&' )
            bf1[ y - 1 ] = '\0';
         for( y = 0; bf1[ y ]; y++ )
         {
            if( bf1[ y ] == '_' )
               bf1[ y ] = ' ';
         }
         strcpy( term[ q ].s, bf1 );
      }
   }

/*   for( q = 0; q < 5; q++ )
      printf( "\nterm %d = *%s*", q, term[ q ].s );
   printf( "\n" ); */

} /* _______________________________________________________ END GETTERMS */


/* ______________________________________________________ FUNCTION FORKIT */

void forkit( void )
{
   int status, retval, cnt, lineno, y, totlines = 0;
   pid_t pid, wpid = 0;
   static FILE *file43;

   /* /scslop/goochkip is a manually-maintained file of working Google
      data centers, one IP address per line. A different program was
      written to check them once a day. An IP address is plucked from
      this file, using procid to pseudo-randomize. As of 2005-03-30
      we know of 88 IP addresses that function in this program:
216.239.37.104
216.239.37.105
216.239.37.106
216.239.37.107
216.239.37.147
216.239.37.99
216.239.39.104
216.239.39.106
216.239.39.107
216.239.39.99
216.239.53.104
216.239.53.106
216.239.53.107
216.239.53.99
216.239.57.104
216.239.57.105
216.239.57.106
216.239.57.107
216.239.57.147
216.239.57.98
216.239.57.99
216.239.59.104
216.239.59.105
216.239.59.106
216.239.59.107
216.239.59.147
216.239.59.99
216.239.63.104
216.239.63.99
64.233.161.104
64.233.161.105
64.233.161.106
64.233.161.107
64.233.161.147
64.233.161.99
64.233.163.104
64.233.163.106
64.233.163.107
64.233.163.99
64.233.167.104
64.233.167.105
64.233.167.106
64.233.167.107
64.233.167.147
64.233.167.99
64.233.171.104
64.233.171.105
64.233.171.106
64.233.171.107
64.233.171.147
64.233.171.99
64.233.179.104
64.233.179.106
64.233.179.107
64.233.179.99
64.233.183.104
64.233.183.106
64.233.183.107
64.233.183.99
64.233.185.104
64.233.185.106
64.233.185.107
64.233.185.99
64.233.187.104
64.233.187.106
64.233.187.107
64.233.187.99
64.233.189.104
66.102.11.104
66.102.11.106
66.102.11.107
66.102.11.99
66.102.7.104
66.102.7.105
66.102.7.106
66.102.7.107
66.102.7.147
66.102.7.99
66.102.9.104
66.102.9.106
66.102.9.107
66.102.9.99
66.249.81.104
66.249.81.99
66.249.85.104
66.249.85.99
66.249.87.104
66.249.87.99
    However, further research suggests that most of these are not used by
    Google to serve search results to the public. Unless you want to show
    up conspicuously in sparse logs, it is safer (at least in the U.S.)
    to use only these from the above list:
216.239.37.99
216.239.39.99
216.239.57.99
216.239.53.104
216.239.57.104
216.239.59.104
216.239.63.104
64.233.161.104
64.233.161.147
64.233.161.99
64.233.167.104
64.233.167.147
64.233.167.99
64.233.179.104
64.233.183.104
64.233.187.104
64.233.187.99
66.102.11.104
66.102.7.104
66.102.7.147
66.102.7.99
66.102.9.104

*/

   lineone[ 0 ] = '\0';
   if( ( file43 = fopen( "/home/slops/scslop/goochkip", "rb" ) ) != NULL )
   {
      fgets( lineone, 29, file43 );
      while( !feof( file43 ) )        /* first we get total lines in file */
      {
         totlines++;
         fgets( lineone, 29, file43 );
      }
      if( !totlines )     /* prevents floating point error if 0-byte file */
         totlines = 1;
      y = procid % totlines;                          /* take the modulus */
      if( !y )
         y = totlines;
      rewind( file43 );       /* then we rewind and grab the line we want */
      lineno = 1;
      fgets( lineone, 29, file43 );
      while( !feof( file43 ) )
      {
         if( lineno == y )
            break;
         lineno++;
         fgets( lineone, 29, file43 );
      }
      fclose( file43 );
   }
   if( lineone[ 0 ] )
   {
      lineone[ strlen( lineone ) - 1 ] = '\0';
      strcpy( resource, "http://" );
      strcat( resource, lineone );
      strcat( resource, "/ie?q=" );
   }
   else
      strcpy( resource, "http://www.google.com/ie?q=" );
   strcat( resource, linebuf2 );
   strcat( resource, "&num" );
   strcat( resource, resnum );
   strcat( resource, "&hl=en&lr=" );
   if( lang[ 0 ] != 'X' )
   {
      strcat( resource, "lang_" );
      strcat( resource, lang );
      if( lang[ 0 ] == 'z' )
      {
         resource[ strlen( resource ) - 1 ] = '\0';
         if( lang[ 1 ] == 's' )
            strcat( resource, "h-CN" );
         else if( lang[ 1 ] == 't' )
            strcat( resource, "h-TW" );
      }
   }
   strcat( resource, "&btnG=Google+Search" );

    /* snarf is used to search Google. Depending on your cgi-bin permissions
       situation, you may need an extra copy of snarf in the user's directory
       who owns the cgi-bin. Otherwise, some cgi-bin wrappers won't see the
       correct permissions on snarf and Apache will most likely abort with
       a "Premature end of script headers" error. Google should receive a
       reasonable user-agent from snarf or whatever is used, or the results
       may return as charset ISO 8859-1 instead of UTF-8 */

   pid = fork();
   if( pid == 0 )
      execl( "/home/dbsc/snarf", "/home/dbsc/snarf", "-nq", resource, wdump, NULL );
   else if( pid == -1 )
      return;
   else
      wpid = pid;

   for( cnt = 1; cnt < 60; cnt++ )
   {
      usleep( 100000 );
      retval = waitpid( -1, &status, WNOHANG );
      if( retval == wpid )
         return;
   }
                   /* after 6 seconds, we still have no sign of returning */
   kill( wpid, SIGKILL );
   sleep( 1 );
   remove( wdump );     /* remove if it's there; no harm done if it isn't */

} /* _________________________________________________________ END FORKIT */


/* ___________________________________________________ FUNCTION LOADCHECK */

void loadcheck( void )
{
  /* /namebase/loadlev is a file with load level thresholds for various
       programs. The one for this program has a 'w' in linebuf[ 7 ]       */

   double y, z;
   unsigned char upline[ 31 ];

   static FILE *file23;
   static FILE *file24;
   static FILE *file89;

   y = 1.00;
   if( ( file23 = fopen( "/home/namebase/loadlev", "rb" ) ) == NULL )
      return;
   fgets( linebuf, 79, file23 );
   while( !feof( file23 ) )
   {
      if( linebuf[ 0 ] == '#' )
      {
         fgets( linebuf, 79, file23 );
         continue;
      }
      if( linebuf[ 7 ] == 'w' )
      {
         if( linebuf[ 9 ] == '=' )
         {
            y = atof( linebuf + 10 );
            break;
         }
      }
      fgets( linebuf, 79, file23 );
   }
   fclose( file23 );

   if( ( file24 = fopen( "/proc/loadavg", "rb" ) ) == NULL )
      return;
   fgets( upline, 19, file24 );
   fclose( file24 );
   z = atof( upline );

   if( z > y )
   {
      fputs( "Content-type: text/html; charset=UTF-8\n\n", stdout );
      fflush( stdout );
      overload();
      strcat( logbuf, savedomain );       /* time stamp already in logbuf */
      sprintf( upline, "  nbbw overload %2.2f\n", z );
      strcat( logbuf, upline );
      if( ( file89 = fopen( "/home/slops/nbslop2/nblog3", "ab" ) ) == NULL )
         return;
      fputs( logbuf, file89 );
      fclose( file89 );
      exit( 0 );
   }

} /* ______________________________________________________ END LOADCHECK */


/* ___________________________________________________ FUNCTION CONCATURL */

void concaturl( void )
{
         /* bufin arrives with a complete URL + anchor, with a item number
            in front. Extract the portion between the quotes and strip off
            the http:// and leave the URL only, with green formatting,
            in magurl variable. In the end, you want it to look like this:
            <br><font color=#008000>www.google-watch.org</font>
            Any magurl that ends up over 780 characters should be nulled. */

   int e, f;
   unsigned char *cpq;

   magurl[ 0 ] = '\0';
   cpq = strchr( bufin, '"' );
   if( cpq == NULL )
      return;
   strcpy( magurl, "<br><font color=#008000>" );
   strncat( magurl, cpq + 8, 1149 );           /* strip "http:// in front */
   f = strlen( magurl );
   if( f < 6 )
   {
      magurl[ 0 ] = '\0';
      return;
   }
   for( e = 3; magurl[ e ] != '"' && magurl[ e ]; e++ )
      ;
   magurl[ e ] = '\0';
   if( e < 6 || e > 773 )
   {
      magurl[ 0 ] = '\0';
      return;
   }
   strcat( magurl, "</font>" );

} /* ______________________________________________________ END CONCATURL */


/* ____________________________________________________ FUNCTION OVERLOAD */

void overload( void )
{
   fputs( "<HTML><HEAD><META HTTP-EQUIV=\"content-type\"  CONTENT=\"text/html; charset=UTF-8\">\n", stdout );
   fputs( "<TITLE>Scroogle proxy is overloaded</TITLE></HEAD><BODY BGCOLOR=#ffffff><BR>\n", stdout );
   fputs( "<CENTER><H1>Our proxy is overloaded!</H1></CENTER>\n", stdout );
   fputs( "<BR></BODY></HTML>\n", stdout );

} /* _______________________________________________________ END OVERLOAD */


/* ____________________________________________________ FUNCTION REPEATER */

void repeater( void )
{

  /*  This allows us to check the /scslop/terms file for duplicate
      searches. Many searchers repeat their search, usually inadvertently.
      By picking up these repeated searches from the local directory,
      it means we don't have to fork to Google again. Only the search
      terms are identified in /scslop/terms, without any IP addresses.
      In the /nbslop2/nblog3 file, only the IP addresses are identified,
      without any search terms. In /scslop/terms, the resnum (number of
      results) is also recorded, to prevent matches for the same terms
      with a different number of results. */

   int slook;
   unsigned char iline[ 601 ], sterm[ 601 ], hitline[ 601 ];
   unsigned char *cpz;
   struct stat ftable;
   static FILE *file87;
   static FILE *file88;

   if( ( file87 = fopen( "/home/slops/scslop/terms", "rb" ) ) == NULL )
      return;
   hitline[ 0 ] = '\0';
   strcpy( sterm, linebuf2 );
   strcat( sterm, resnum );                         /* =100 or =50 or =20 */
   strcat( sterm, lang );
   strcat( sterm, "  wgn" );
   slook = strlen( sterm );
                   /* we only check the last 5000 bytes of the file (this
                         number depends on your expected average traffic) */
   if( fstat( fileno( file87 ), &ftable ) )
   {
      fclose( file87 );
      return;
   }
   if( ftable.st_size > 5000 )
   {   
      fseek( file87, ftable.st_size - 4999, SEEK_SET );   
      fgets( iline, 599, file87 );              /* throw out partial line */
   }
   fgets( iline, 599, file87 );                        /* first full line */
   while( !feof( file87 ) )
   {
          /* don't break on a match because we want the latest one in the
            file; earlier ones may already be deleted by our cron program */
      if( !strncmp( iline + 13, sterm, slook ) )
         strcpy( hitline, iline + 13 );
      fgets( iline, 599, file87 );
   }
   fclose( file87 );

   if( !hitline[ 0 ] )                 /* if no match, we add to the file */
   {
      if( ( file88 = fopen( "/home/slops/scslop/terms", "ab" ) ) == NULL )
         return;
      strcpy( linebuf, logbuf );
      strcat( linebuf, linebuf2 );
      strcat( linebuf, resnum );                    /* =100 or =50 or =20 */
      strcat( linebuf, lang );
      strcat( linebuf, "  " );
      strcat( linebuf, wdump + 19 );  /* skip the path for a smaller file */
      strcat( linebuf, cr );
      fputs( linebuf, file88 );
      fclose( file88 );
      return;
   }
      /* There's an old filename that we can use for wdump without doing
           another Google search. The name of the old file is in hitline. */
   cpz = strstr( hitline, "  wgn" );
   if( cpz == NULL )
      return;
   strcpy( iline, cpz + 2 );
   iline[ strlen( iline ) - 1 ] = '\0';
   strcpy( hitline, wdump );         /* save old wdump in case we need it */
   strcpy( wdump, "/home/slops/scslop/" );            /* restore the path */
   strcat( wdump, iline );
   if( ( file87 = fopen( wdump, "rb" ) ) != NULL )        /* still there? */
   {
      fclose( file87 );
      oldsearch = 1;
   }
   else
   {
      strcpy( wdump, hitline );                 /* restore original wdump */
      if( ( file88 = fopen( "/home/slops/scslop/terms", "ab" ) ) == NULL )
         return;
      strcpy( linebuf, logbuf );
      strcat( linebuf, linebuf2 );
      strcat( linebuf, resnum );
      strcat( linebuf, lang );
      strcat( linebuf, "  " );
      strcat( linebuf, wdump + 19 );
      strcat( linebuf, cr );
      fputs( linebuf, file88 );
      fclose( file88 );
   }

} /* _______________________________________________________ END REPEATER */


/* ___________________________________________________ FUNCTION DRIVEUTF8 */

void driveutf8( void )
{
   int q, r, s, t, u;
   unsigned char numst[ 12 ];
   unsigned char *cp20, *cp21;

   strcpy( bufout, linebuf );
   cp20 = strstr( bufout, "&amp;" );          /* replace all &amp; with & */
   if( cp20 == NULL )
   {
      cp21 = strchr( bufout, '&' );
      if( cp21 == NULL )
         return;                                         /* nothing to do */
   }
   while( cp20 != NULL )
   {
      *( cp20 + 1 ) = '\0';
      strcat( bufout, cp20 + 5 );
      cp20 = strstr( bufout, "&amp;" );
   }
                                     /* now bufout has '&' but no "&amp;" */
   cp20 = strstr( bufout, "&#" );         /* this marks a Unicode decimal */
   if( cp20 == NULL )
     return;                                             /* nothing to do */

               /* extract "&#nnn;" as nnn, append the UTF-8 encoded bytes
                   for nnn in place of "&#nnn;" and keep looking for more */
   r = 0;
   for( q = 0; bufout[ q ]; q++ )
   {
      if( bufout[ q ] == '&' && bufout[ q + 1 ] == '#' )
      {
         strncpy( numst, bufout + q + 2, 9 );
         numst[ 9 ] = '\0';
         cp20 = strchr( numst, ';' );
         if( cp20 == NULL )
         {
            linebuf[ r ] = bufout[ q ];        /* not a number we can use */
            r++;
            continue;
         }
         else
            *( cp20 ) = '\0';                 /* now it might be a number */
         u = 0;
         for( t = 0; numst[ t ]; t++ )            /* check for all digits */
         {
            if( !isdigit( numst[ t ] ) )
               u = 1;
         }
         if( !u )
            s = atoi( numst );                           /* valid integer */
         if( u || s > 65535 )
         {
            linebuf[ r ] = bufout[ q ];        /* not a number we can use */
            r++;
            continue;
         }
         numtobin( s );
         for( t = 0; rawutf[ t ]; t++ )        /* append UTF-8 characters */
         {
            linebuf[ r ] = rawutf[ t ];
            r++;
         }
         while( bufout[ q ] && bufout[ q ] != ';' )         /* skip ahead */
            q++;
      }
      else
      {
         linebuf[ r ] = bufout[ q ];
         r++;
      }
   }
   linebuf[ r ] = '\0';

} /* ______________________________________________________ END DRIVEUTF8 */


/* ____________________________________________________ FUNCTION NUMTOBIN */

void numtobin( int c )
{
   int a, b, v;
   long z;
   char *endptr;
   unsigned char buf1[ 25 ], buf2[ 35 ];

                    /* This function takes a Unicode decimal integer and
                       converts it to a UTF-8 multibyte string in rawutf. */
   v = c;
   buf1[ 24 ] = '\0';

   if( c < 128 )
   {
      rawutf[ 0 ] = (unsigned char)c;
      rawutf[ 1 ] = '\0';
      return;
   }

   for( a = 23; a > -1 ; a-- )     /* converts c to a string of 0s and 1s */
   {
      b = v % 2;
      if( b )
         buf1[ a ] = '1';
      else
         buf1[ a ] = '0';
      v /= 2;
   }

   if( c < 2048 )
   {
      strcpy( buf2, "110" );
      strncat( buf2, buf1 + 13, 5 );
      z = strtol( buf2, &endptr, 2 );
      rawutf[ 0 ] = (unsigned char)z;
      strcpy( buf2, "10" );
      strncat( buf2, buf1 + 18, 6 );
      z = strtol( buf2, &endptr, 2 );
      rawutf[ 1 ] = (unsigned char)z;
      rawutf[ 2 ] = '\0';
      return;
   }
   if( c < 65536 )
   {
      strcpy( buf2, "1110" );
      strncat( buf2, buf1 + 8, 4 );
      z = strtol( buf2, &endptr, 2 );
      rawutf[ 0 ] = (unsigned char)z;
      strcpy( buf2, "10" );
      strncat( buf2, buf1 + 12, 6 );
      z = strtol( buf2, &endptr, 2 );
      rawutf[ 1 ] = (unsigned char)z;
      strcpy( buf2, "10" );
      strncat( buf2, buf1 + 18, 6 );
      z = strtol( buf2, &endptr, 2 );
      rawutf[ 2 ] = (unsigned char)z;
      rawutf[ 3 ] = '\0';
   }

} /* _______________________________________________________ END NUMTOBIN */
