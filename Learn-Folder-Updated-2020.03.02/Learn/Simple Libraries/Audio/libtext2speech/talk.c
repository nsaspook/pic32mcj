//
// automatically generated by spin2cpp v1.93 on Thu Oct 08 13:24:57 2015
// spin2cpp --ccode --main talk_demo2.spin 
//

// Phonemic voice synthesizer.
// 
// Copyright (C) 2006 Philip C. Pilgrim
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// CONTACT
//
//  phil@phipi.com (First contact may require addition to my whitelist.)
//
// VERSION HISTORY
//
//  0.01 alpha: released 2006.10.31
//
//  0.02 alpha: released 2006.11.04
//
//    Improved k and z sounds; added whispering, medium pause, spell function.
//
//  0.03 alpha: released 2006.11.07
//
//    Added inflections via sliding pitch (/\) during vowel pronunciation.
//    Added accents ('), which combine the [ and \ functions.
//    Added rolled R (rr) phoneme.
//    Added music notation (CDEFGABC) to make songs more readable.
//    Added tempo adjustment (%).
//    Added multiple speaker support (#) (pitch only).
//
// SUMMARY
//
// This program is an attempt to use Chip Gracey's VocalTract module to produce phonemic-based speech.
// The formant values it uses are based heavily on work done by D.H. Klatt in the "KLSYN" formant
// synthesis program.
//
// This program's "say" function accepts a string of bytes that represent English phonemes.
// It "speaks" them on the port given in the argument to "start". The string can include the following
// one- and two-character combinations:
//
//    #         Select speaker 0.
//    #n        Select speaker n (0 =< n =< 9).
//    _         Set the glottal pitch to the base frequency.
//    [         Raise glottal pitch by one semitone.
//    ]         Lower glottal pitch by one semitone.
//    /         Slide the glottal pitch up by one semitone during the next frame.
//    \         Slide the glottal pitch down by one semitone during the next frame.
//    '         Accent the next frame (same as +\).
//    +         Raise the glottal pitch by one semitone. (i.e. "Sharp" the previous note.)
//    +n        Raise the glottal pitch by n semitones (n = "1" to "9").
//    -         Lower the glottal pitch by one semitone. (i.e. "Flat" the previous note.)
//    -n        Lower the glottal pitch by n semitones (n = "1" to "9").
//    A .. G    Set the glottal pitch to the indicated note in the current octave.
//    An .. Gn  Set the glottal pitch to the indicated note in octave n (n = "0" to "9").
//    %         Set the tempo to 100%
//    %nnnn     Set the tempo to nnnn% (25 =< nnnn =< 1600) Larger numbers = slower tempo.
//    <         Raise the volume by one notch.
//    >         Lower the volume by one notch.
//    ~         Add a short "uh" to the end of the last letter (usually a consonant) for emphasis.
//    ,         Short pause.
//    ;         Medium pause.
//    .         Long pause.
//    (         Begin whispering.
//    )         End whispering.
//    |         Do not interpolate between previous and next phonemes.
//    <blank>   Syntactic separator. Prevents adjacent letters from being treated as a pair.
//    a         A as in At.
//    ae        Long I sound, as in bIte.
//    al        AL, as in pAL.
//    ai,ay     Long A sound, as in bAIt.
//    ar        AR, as in pARt.
//    ah,o      "ah" sound, as in pOt.
//    e         Short E sound, as in pEt.
//    ee        Long E sound, as in fEEd.
//    ew        EW, as in pEW.
//    er,ir     IR, as in gIRl 
//    el        EL, as in pELt.
//    i         Short I sound, as in hIt.
//    o         Short O sound, as in pOt.
//    oa        Long O sound, as in bOAt.
//    or        OR, as in fORt.
//    ol        "ahl" sound, as in mALl
//    ou,ow     OU, as in OUt.
//    oi,oy     OY, as in bOY.
//    oo        Long OO sound, as in bOOt.
//    u         Short U sound, as in gUt.
//    uu        Short OO sound, as in pUt.
//    d         D, as in Dog.
//    dh        Soft (voiced) TH sound, as in THat.
//    t         T, as in Tot.
//    th        Hard (unvoiced) TH sound, as in THink.
//    s         S, as in Sip.
//    sh        SH, as in SHip.
//    c,k       C, as in Cot.
//    ch        CH, as in CHip.
//    z         Z, as in Zoo.
//    zh        ZH sound, as in aZure.
//    l         L, as in Lot.
//    r         R, as in Rot.
//    rr        Rolled R, as in peRRo (Esp.).
//    w         W, as in Want.
//    y         Y, as in Yell.
//    m         M, as in Mom.
//    n         N, as in Name.
//    p         P, as in Pop.
//    g         G, as in Good.
//    b         B, as in Bad.
//    f         F, as in Fad.
//    h         H, as in Had.
//    v         V, as in Vat.
//    j         J, as in Job.
//
// The "set" procedure sets the vocal tract parameters based on its list of arguments. It uses a crude audio
// "kerning" function, wherein each frame can have a lead-in time, a play time, and a lead-out time. Lead-in
// and lead-out determine the amount of blending that occurs between frames. The actual lead-in time used is
// the lesser of a frame's designated lead-in time and the lead-out time of the previous frame. 
#include <stdlib.h>
#include <propeller.h>
#include "text2speech.h"

#ifdef __GNUC__
#define INLINE__ static inline
#define PostEffect__(X, Y) __extension__({ int32_t tmp__ = (X); (X) = (Y); tmp__; })
#else
#define INLINE__ static
static int32_t tmp__;
#define PostEffect__(X, Y) (tmp__ = (X), (X) = (Y), tmp__)
#define waitcnt(n) _waitcnt(n)
#define coginit(id, code, par) _coginit((unsigned)(par)>>2, (unsigned)(code)>>2, id)
#define cognew(code, par) coginit(0x8, (code), (par))
#define cogstop(i) _cogstop(i)
#endif

INLINE__ int32_t Min__(int32_t a, int32_t b) { return a < b ? a : b; }
INLINE__ int32_t Max__(int32_t a, int32_t b) { return a > b ? a : b; }
#include <setjmp.h>
typedef struct { jmp_buf jmp; int32_t val; } AbortHook__;
AbortHook__ *abortChain__ __attribute__((common));

INLINE__ int32_t Shr__(uint32_t a, uint32_t b) { return (a>>b); }
INLINE__ int32_t Between__(int32_t x, int32_t a, int32_t b){ if (a <= b) return x >= a && x <= b; return x >= b && x <= a; }

static  int32_t talk_formant( talk *self, int32_t sf1, int32_t sf2, int32_t sf3, int32_t sf4);
static  int32_t talk_set( talk *self, int32_t fmt, int32_t ptr, int32_t pre, int32_t time, int32_t post);

static uint8_t dat[] = {
  0x80, 0x00, 0x8c, 0x00, 0x98, 0x00, 0xa6, 0x00, 0xb5, 0x00, 0xc5, 0x00, 0xd7, 0x00, 0xeb, 0x00, 
  0x00, 0x01, 0x17, 0x01, 0x30, 0x01, 0x4c, 0x01, 0x6a, 0x01, 0x8b, 0x01, 0xaf, 0x01, 0xd6, 0x01, 
  0x00, 0x02, 
};
talk *talk_run(int pin, int npin)
{
  talk *self = (talk *)malloc(sizeof(talk));
  talk_end(self);
  self->vocal_cog = VocalTract_start(&self->v, (int32_t)(&self->vt[0]), pin, npin, (-1));
  memset( (void *)&self->speaker[0], 100, 1*(10));
  self->base_freq = 100;
  return self;
}

void talk_end(talk *self)
{
  if (self->vocal_cog) {
    cogstop((self->vocal_cog - 1));
    self->vocal_cog = 0;
    free(self);
  }
  // return 0;
}

int32_t talk_set_speaker(talk *self, int32_t spkr, int32_t base)
{
  // Assign a base pitch to speaker #spkr.
  self->speaker[(Min__((Max__(spkr, 0)), 9))] = base;
  return 0;
}

int32_t talk_spell(talk *self, char *ptr)
{
  int32_t	Char;
  char *s;
  while ((Char = ((uint8_t *)(ptr++))[0])) {
    if ((Char >= 'A') && (Char <= 'Z')) {
      talk_say(self, "k\'apitul");
      Char = Char + 32;
    }
    if (Char == ' ') {
      s = "spays";
    } else if (Char == 'a') {
      s = "ay";
    } else if (Char == 'b') {
      s = "bee";
    } else if (Char == 'c') {
      s = "see";
    } else if (Char == 'd') {
      s = "dee";
    } else if (Char == 'e') {
      s = "ee";
    } else if (Char == 'f') {
      s = "ef";
    } else if (Char == 'g') {
      s = "jee";
    } else if (Char == 'h') {
      s = "aych";
    } else if (Char == 'i') {
      s = "ae";
    } else if (Char == 'j') {
      s = "jay";
    } else if (Char == 'k') {
      s = "kay";
    } else if (Char == 'l') {
      s = "el";
    } else if (Char == 'm') {
      s = "em~";
    } else if (Char == 'n') {
      s = "en~";
    } else if (Char == 'o') {
      s = "oa";
    } else if (Char == 'p') {
      s = "pee";
    } else if (Char == 'q') {
      s = "kew";
    } else if (Char == 'r') {
      s = "ar";
    } else if (Char == 's') {
      s = "ess";
    } else if (Char == 't') {
      s = "tee";
    } else if (Char == 'u') {
      s = "yoo";
    } else if (Char == 'v') {
      s = "vee";
    } else if (Char == 'w') {
      s = "d\'ubulyoo";
    } else if (Char == 'x') {
      s = "eks";
    } else if (Char == 'y') {
      s = "wae";
    } else if (Char == 'z') {
      s = "zee";
    } else if (Char == '0') {
      s = "z\'eeroa";
    } else if (Char == '1') {
      s = "wun~";
    } else if (Char == '2') {
      s = "too";
    } else if (Char == '3') {
      s = "three";
    } else if (Char == '4') {
      s = "for";
    } else if (Char == '5') {
      s = "faev";
    } else if (Char == '6') {
      s = "siks";
    } else if (Char == '7') {
      s = "s\'even~";
    } else if (Char == '8') {
      s = "ayt";
    } else if (Char == '9') {
      s = "naen~";
    } else if (Char == '.') {
      s = "p\'eereeud";
    } else if (Char == ',') {
      s = "k\'omu";
    } else if (Char == ':') {
      s = "k\'oalun~";
    } else if (Char == '?') {
      s = "kw\'estshun mark~";
    } else if (Char == '!') {
      s = "\'eksklam\'ayshun point";
    } else if (Char == ';') {
      s = "s\'emaekoalun~";
    } else if (Char == '\'') {
      s = "uhp\'ostrufee";
    } else if (Char == '@') {
      s = "at";
    } else if (Char == '#') {
      s = "n\'umber";
    } else if (Char == '$') {
      s = "d\'olur saen";
    } else if (Char == '%') {
      s = "pers\'ent";
    } else if (Char == '^') {
      s = "k\'a rut";
    } else if (Char == '&') {
      s = "\'ampersand";
    } else if (Char == '*') {
      s = "star";
    } else if (Char == '(') {
      s = "left pur\'enthesis";
    } else if (Char == ')') {
      s = "raet pur\'enthesis";
    } else if (Char == '-') {
      s = "haefun";
    } else if (Char == '_') {
      s = "\'underskor";
    } else if (Char == '+') {
      s = "plus";
    } else if (Char == '=') {
      s = "\'eekwol saen~";
    } else if (1) {
      s = "\'unoan k\'a racter";
    }
    talk_say(self, s);
    talk_say(self, ";");
  }
  return 0;
}

int32_t talk_say(talk *self, char *ptr)
{
  int32_t	This, nxt, octave;
  int32_t result = 0;
  if (self->vocal_cog == 0) {
    return result;
  }
  self->vt[GP] = (self->vt[GPS] = self->base_freq);
  self->initial_k = (self->initial_g = (self->aspirate = (self->whisper = 0)));
  self->volume = 6;
  self->dilate = 100;
  talk_set(self, talk_formant(self, 650, 990, 2530, 3480), (int32_t)"\022", 0, 10, 0);
  while ((This = ((uint8_t *)(ptr++))[0])) {
    if (This == '_') {
      self->vt[GP] = (self->vt[GPS] = self->base_freq);
    } else if (This == '/') {
      self->vt[GPS] = self->vt[GPS] + 4;
    } else if (This == '\\') {
      self->vt[GPS] = self->vt[GPS] - 4;
    } else if (This == '[') {
      self->vt[GPS] = (self->vt[GP] = self->vt[GP] + 4);
    } else if (This == ']') {
      self->vt[GPS] = (self->vt[GP] = self->vt[GP] - 4);
    } else if (This == '\'') {
      self->vt[GP] = self->vt[GP] + 4;
    } else if (This == ',') {
      talk_set(self, 0, (int32_t)"\022", 100, 200, 0);
    } else if (This == ';') {
      talk_set(self, 0, (int32_t)"\022", 100, 450, 0);
    } else if (This == '.') {
      talk_set(self, 0, (int32_t)"\022", 100, 750, 0);
    } else if (This == '|') {
      self->glide = 0;
    } else if (This == '(') {
      self->whisper = -1;
    } else if (This == ')') {
      self->whisper = 0;
    } else if (This == '%') {
      self->dilate = 0;
      while (((nxt = ((uint8_t *)(ptr++))[0]) >= '0') && (nxt <= '9')) {
        self->dilate = ((self->dilate * 10) + nxt) - '0';
      }
      (ptr--);
      if (self->dilate) {
        self->dilate = Min__((Max__(self->dilate, 25)), 1600);
      } else {
        self->dilate = 100;
      }
    } else if (This == '~') {
      talk_set(self, talk_formant(self, 640, 1200, 2400, 3000), (int32_t)"\016\017", 10, 15, 10);
    } else if (This == '#' || This == '_' || Between__(This, 'A', 'G') || This == '+' || This == '-' || This == '=' || This == '<' || This == '>' || This == 'a' || This == 'e' || This == 'i' || This == 'o' || This == 'u' || This == 'd' || This == 't' || This == 's' || This == 'c' || This == 'k' || This == 'g' || This == 'z' || This == 'r') {
      nxt = ((uint8_t *)(ptr++))[0];
      if (This == '#') {
        if (Between__(nxt, '0', '9')) {
          self->base_freq = self->speaker[(nxt - '0')];
        } else if (1) {
          (ptr--);
          self->base_freq = self->speaker[0];
        }
        self->vt[GP] = (self->vt[GPS] = self->base_freq);
      } else if (Between__(This, 'A', 'G')) {
        if (Between__(nxt, '1', '5')) {
          octave = nxt - '0';
        } else if (1) {
          (ptr--);
          octave = (self->vt[GP] + 32) / 48;
        }
        self->vt[GP] = (self->vt[GPS] = Min__((((octave - 1) * 48) + ((uint8_t *)(int32_t)"4<\020\030 $,")[(This - 'A')]), 255));
      } else if (This == '+' || This == '-') {
        if (Between__(nxt, '1', '9')) {
          self->vt[GP] = self->vt[GP] + (((',' - This) * (nxt - '0')) << 2);
        } else if (1) {
          (ptr--);
          self->vt[GP] = self->vt[GP] + ((',' - This) << 2);
        }
        self->vt[GPS] = self->vt[GP];
      } else if (This == '=') {
        if (Between__(nxt, '0', '9')) {
          self->volume = nxt - '0';
        } else if (1) {
          (ptr--);
          self->volume = 6;
        }
      } else if (This == '<' || This == '>') {
        if (Between__(nxt, '0', '9')) {
          self->volume = Max__((Min__((self->volume + (('=' - This) * (nxt - '0'))), 9)), 0);
        } else if (1) {
          (ptr--);
          self->volume = Max__((Min__(((self->volume + '=') - This), 9)), 0);
        }
      } else if (This == 'a') {
        if (nxt == 'e') {
          /* fIne */
          talk_set(self, talk_formant(self, 650, 990, 2530, 3480), 0, 10, 1, 300);
          talk_set(self, talk_formant(self, 310, 2020, 2960, 3500), 0, 300, 50, 100);
        } else if (nxt == 'l') {
          /* pAL */
          talk_set(self, talk_formant(self, 660, 1700, 2400, 3900), 0, 200, 200, 100);
          talk_set(self, talk_formant(self, 310, 1050, 2880, 3500), 0, 100, 200, 50);
        } else if (nxt == 'i' || nxt == 'y') {
          /* mAIl */
          talk_set(self, talk_formant(self, 660, 1700, 2400, 3900), 0, 200, 0, 150);
          talk_set(self, talk_formant(self, 310, 2020, 2960, 3500), 0, 150, 200, 100);
        } else if (nxt == 'r') {
          /* pARt */
          talk_set(self, talk_formant(self, 650, 990, 2530, 3480), 0, 200, 100, 100);
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), 0, 100, 200, 100);
          talk_set(self, talk_formant(self, 310, 1060, 1380, 3500), (int32_t)"\022", 100, 200, 200);
        } else if (nxt == 'h') {
          /* pOt (same as "o") */
          talk_set(self, talk_formant(self, 650, 990, 2530, 3480), 0, 200, 200, 100);
        } else if (1) {
          /* pAt */
          (ptr--);
          talk_set(self, talk_formant(self, 660, 1700, 2400, 3900), 0, 200, 200, 100);
        }
      } else if (This == 'e') {
        if (nxt == 'e') {
          /* wEE */
          talk_set(self, talk_formant(self, 310, 2020, 2960, 3500), 0, 100, 200, 100);
        } else if (nxt == 'w') {
          /* fEW */
          talk_set(self, talk_formant(self, 310, 2020, 2960, 3500), 0, 100, 0, 150);
          talk_set(self, talk_formant(self, 300, 870, 2250, 3900), 0, 200, 100, 100);
        } else if (nxt == 'r') {
          /* fERn */
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), 0, 100, 200, 100);
          talk_set(self, talk_formant(self, 310, 1060, 1380, 3500), (int32_t)"\022", 100, 200, 200);
        } else if (nxt == 'l') {
          /* fELL */
          talk_set(self, talk_formant(self, 580, 1799, 2605, 3677), 0, 100, 100, 100);
          talk_set(self, talk_formant(self, 310, 1050, 2880, 3500), 0, 100, 200, 50);
        } else if (1) {
          /* pEt */
          (ptr--);
          talk_set(self, talk_formant(self, 580, 1799, 2605, 3677), 0, 100, 100, 100);
        }
      } else if (This == 'i') {
        if (nxt == 'r') {
          /* gIRl (same as "er") */
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), 0, 100, 200, 100);
          talk_set(self, talk_formant(self, 310, 1060, 1380, 3500), (int32_t)"\022", 100, 200, 200);
        } else if (1) {
          /* pIt */
          (ptr--);
          talk_set(self, talk_formant(self, 400, 2000, 2550, 3900), 0, 100, 150, 100);
        }
      } else if (This == 'o') {
        if (nxt == 'a') {
          /* bOAt */
          talk_set(self, talk_formant(self, 640, 1200, 2400, 3000), 0, 100, 0, 150);
          talk_set(self, talk_formant(self, 300, 870, 2250, 3900), 0, 150, 100, 100);
        } else if (nxt == 'r') {
          /* fOR */
          talk_set(self, talk_formant(self, 640, 1200, 2400, 3000), 0, 100, 0, 100);
          talk_set(self, talk_formant(self, 300, 870, 2250, 3900), 0, 100, 0, 100);
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), (int32_t)"\016\024", 100, 0, 50);
          talk_set(self, talk_formant(self, 310, 1060, 1380, 3500), (int32_t)"\022", 50, 200, 200);
        } else if (nxt == 'l') {
          /* mALL */
          talk_set(self, talk_formant(self, 650, 990, 2530, 3480), 0, 200, 200, 100);
          talk_set(self, talk_formant(self, 310, 1050, 2880, 3500), 0, 100, 200, 50);
        } else if (nxt == 'u' || nxt == 'w') {
          /* pOUt */
          talk_set(self, talk_formant(self, 660, 1700, 2400, 3900), 0, 100, 100, 300);
          talk_set(self, talk_formant(self, 300, 870, 2250, 3900), 0, 300, 50, 100);
        } else if (nxt == 'i' || nxt == 'y') {
          /* bOY */
          talk_set(self, talk_formant(self, 640, 1200, 2400, 3000), 0, 100, 0, 150);
          talk_set(self, talk_formant(self, 300, 870, 2250, 3900), 0, 150, 0, 200);
          talk_set(self, talk_formant(self, 310, 2020, 2960, 3500), 0, 200, 50, 100);
        } else if (nxt == 'o') {
          /* bOOt */
          talk_set(self, talk_formant(self, 300, 870, 2250, 3900), 0, 100, 200, 100);
        } else if (1) {
          /* pOt (same as "ah") */
          (ptr--);
          talk_set(self, talk_formant(self, 650, 990, 2530, 3480), 0, 200, 200, 100);
        }
      } else if (This == 'u') {
        if (nxt == 'u') {
          /* pUt */
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), 0, 100, 200, 100);
        } else if (1) {
          /* pUtt */
          (ptr--);
          talk_set(self, talk_formant(self, 640, 1200, 2400, 3000), 0, 100, 200, 100);
        }
      } else if (This == 'd') {
        if (nxt == 'h') {
          talk_set(self, 0, (int32_t)"\022", 0, 30, 0);
          talk_set(self, talk_formant(self, 350, 1800, 2820, 3400), (int32_t)"\022\016\005\020\005\014\372", 0, 100, 0);
        } else if (1) {
          /* Dot */
          (ptr--);
          talk_set(self, talk_formant(self, 400, 1600, 2600, 3500), (int32_t)"\022", 0, 100, 0);
          talk_set(self, 0, (int32_t)"\r\310", 0, 10, 10);
          talk_set(self, 0, (int32_t)"\r\024\0172\n\024", 100, 100, 100);
        }
      } else if (This == 't') {
        if (nxt == 'h') {
          talk_set(self, 0, (int32_t)"\022", 0, 30, 0);
          talk_set(self, talk_formant(self, 350, 1800, 2820, 3400), (int32_t)"\022\020\n\014\372", 0, 200, 50);
        } else if (1) {
          /* Tot */
          (ptr--);
          talk_set(self, talk_formant(self, 400, 1600, 2600, 3500), (int32_t)"\022", 0, 100, 0);
          talk_set(self, 0, (int32_t)"\022\r\310", 0, 10, 10);
          talk_set(self, 0, (int32_t)"\022\r\024\0172\n\024", 100, 100, 100);
        }
      } else if (This == 's') {
        if (nxt == 'h') {
          talk_set(self, 0, (int32_t)"\022", 0, 30, 0);
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), (int32_t)"\022\r\n\020\005\0142", 0, 200, 0);
          talk_set(self, 0, (int32_t)"\022", 0, 1, 50);
        } else if (1) {
          /* Sit */
          (ptr--);
          talk_set(self, 0, (int32_t)"\022", 0, 30, 0);
          talk_set(self, talk_formant(self, 19, 38, 57, 57), (int32_t)"\022\020\002\014d", 0, 200, 0);
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), (int32_t)"\016\377", 0, 1, 50);
        }
      } else if (This == 'c' || This == 'k') {
        if ((This == 'c') && (nxt == 'h')) {
          talk_set(self, 0, (int32_t)"\022", 0, 30, 0);
          talk_set(self, talk_formant(self, 260, 2070, 3020, 3500), (int32_t)"\022\014d\020\005", 0, 100, 200);
          talk_set(self, 0, (int32_t)"\016\377", 0, 1, 50);
        } else {
          (ptr--);
          if (nxt == 'a' || nxt == 'e' || nxt == 'i' || nxt == 'o' || nxt == 'u') {
            /* Cat */
            self->initial_k = -1;
          } else if (1) {
            talk_set(self, talk_formant(self, 50, 1750, 1750, 3500), (int32_t)"\022", 50, 1, 1);
            talk_set(self, 0, (int32_t)"\022", 10, 1, 1);
            talk_set(self, 0, (int32_t)"\022\020\036\014Z", 0, 15, 0);
            talk_set(self, 0, (int32_t)"\022", 0, 40, 0);
          }
        }
      } else if (This == 'g') {
        (ptr--);
        if (nxt == 'a' || nxt == 'e' || nxt == 'i' || nxt == 'o' || nxt == 'u') {
          /* Got */
          self->initial_g = -1;
        } else if (1) {
          talk_set(self, talk_formant(self, 300, 1990, 2850, 3500), (int32_t)"\022\016\024\r\310", 0, 10, 10);
          talk_set(self, 0, (int32_t)"\022\016\024\r\024\0172\n\024", 40, 40, 10);
        }
      } else if (This == 'z') {
        if (nxt == 'h') {
          talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), (int32_t)"\022\016\024\r\n\020\005\0142", 50, 300, 0);
        } else if (1) {
          /* Zoo */
          (ptr--);
          talk_set(self, talk_formant(self, 150, 1400, 2300, 3180), (int32_t)"\014\372\020\n\016\024", 200, 100, 200);
        }
      } else if (This == 'r') {
        if (nxt == 'r') {
          /* peRRo (Esp.) */
          {
            int32_t _idx__0000;
            for(_idx__0000 = 0; _idx__0000 < 3; _idx__0000++) {
              talk_set(self, talk_formant(self, 310, 1060, 1380, 3500), (int32_t)"\022", 0, 50, 200);
              talk_set(self, talk_formant(self, 640, 1200, 2400, 3000), (int32_t)"\016\017", 10, 15, 10);
            }
          }
        } else if (1) {
          /* Rot */
          (ptr--);
        }
        talk_set(self, talk_formant(self, 310, 1060, 1380, 3500), (int32_t)"\022", 0, 50, 200);
      }
    } else if (1) {
      if (This == 'l') {
        /* Lot */
        talk_set(self, talk_formant(self, 310, 1050, 2880, 3500), 0, 0, 200, 50);
      } else if (This == 'w') {
        talk_set(self, talk_formant(self, 290, 610, 2150, 3500), (int32_t)"\022", 0, 50, 200);
      } else if (This == 'y') {
        talk_set(self, talk_formant(self, 310, 2020, 2960, 3500), (int32_t)"\022", 0, 100, 200);
      } else if (This == 'm') {
        talk_set(self, talk_formant(self, 480, 1270, 2130, 3500), (int32_t)"\022\016\005\017\005\n\016", 10, 200, 30);
      } else if (This == 'n') {
        talk_set(self, talk_formant(self, 480, 1340, 2470, 3500), (int32_t)"\022\016\005\017\005\n\016", 10, 200, 30);
      } else if (This == 'p') {
        talk_set(self, talk_formant(self, 400, 1100, 2150, 3500), (int32_t)"\022", 0, 100, 0);
        talk_set(self, 0, (int32_t)"\022\r<", 0, 10, 10);
        talk_set(self, 0, (int32_t)"\022\r\024\0172\n\024", 100, 100, 100);
      } else if (This == 'b') {
        talk_set(self, talk_formant(self, 200, 1100, 2150, 3500), (int32_t)"\022", 0, 100, 10);
        talk_set(self, 0, (int32_t)"\022\016\n", 10, 40, 10);
      } else if (This == 'f') {
        talk_set(self, 0, (int32_t)"\022", 0, 30, 0);
        talk_set(self, talk_formant(self, 19, 38, 57, 57), (int32_t)"\022\020\002\014\372", 0, 200, 0);
        talk_set(self, talk_formant(self, 470, 1120, 2430, 3400), (int32_t)"\016\377", 0, 1, 50);
      } else if (This == 'h') {
        /* Hit */
        self->aspirate = -1;
      } else if (This == 'v') {
        talk_set(self, talk_formant(self, 220, 1100, 2080, 3500), (int32_t)"\022\014\372\020\002", 0, 100, 200);
      } else if (This == 'j') {
        talk_set(self, talk_formant(self, 260, 2070, 3020, 3500), (int32_t)"\022\016\n\014d\020\n", 0, 150, 100);
      }
    }
  }
  talk_set(self, 0, (int32_t)"\022", 0, 1, 0);
  return result;
}

static int32_t talk_formant(talk *self, int32_t sf1, int32_t sf2, int32_t sf3, int32_t sf4)
{
  if (sf1) {
    self->vt[F1] = sf1 / 19;
  }
  if (sf2) {
    self->vt[F2] = sf2 / 19;
  }
  if (sf3) {
    self->vt[F3] = sf3 / 19;
  }
  if (sf4) {
    self->vt[F4] = sf4 / 19;
  }
  return 0;
}

static int32_t talk_set(talk *self, int32_t fmt, int32_t ptr, int32_t pre, int32_t time, int32_t post)
{
  int32_t	This, nxt, vol;
  if (ptr) {
    while ((This = ((uint8_t *)(ptr++))[0])) {
      if (This == ZERO) {
        This = 0;
      }
      if (This == QT) {
        self->vt[AA] = (self->vt[GA] = (self->vt[NA] = (self->vt[FA] = 0)));
      } else {
        if ((nxt = ((uint8_t *)(ptr++))[0])) {
          if (nxt == ZERO) {
            nxt = 0;
          }
          self->vt[This] = nxt;
        } else {
          if (!abortChain__) abort();
          abortChain__->val =  0;
          longjmp(abortChain__->jmp, 1);
        }
      }
    }
  }
  if ((pre) || (time)) {
    if (self->aspirate) {
      self->vt[AAZ] = (self->vt[GAZ] = 0);
      VocalTract_go(&self->v, 1);
      self->vt[AAZ] = 10;
      VocalTract_go(&self->v, (Max__(((200 * self->dilate) / 100), 1)));
      self->glide = 0;
      self->aspirate = 0;
    } else {
      if ((self->initial_k) || (self->initial_g)) {
        memcpy( (void *)&self->vtp[0], (void *)&self->vt[0], 1*(18));
        if (self->initial_g) {
          self->vt[F1] = Shr__(self->vtp[F1], 2);
          self->vt[F2] = (self->vt[F3] = Shr__((self->vtp[F2] + self->vtp[F3]), 1));
        } else {
          self->vt[F1] = Shr__((self->vt[F1] * 3), 2);
          self->vt[F2] = Shr__(((self->vtp[F2] * 3) + self->vtp[F3]), 2);
          self->vt[F3] = Shr__(((self->vtp[F3] * 3) + self->vtp[F2]), 2);
        }
        self->initial_k = 0;
        self->initial_g = 0;
        talk_set(self, 0, (int32_t)"\022", 10, 1, 1);
        talk_set(self, 0, (int32_t)"\022\020\036\014Z", 0, 15, 0);
        talk_set(self, 0, (int32_t)"\022", 0, 40, 0);
        talk_set(self, 0, (int32_t)"\016\002", 0, 10, 100);
        memcpy( (void *)&self->vt[0], (void *)&self->vtp[0], 1*(18));
      }
    }
    if (self->whisper) {
      self->vt[AA] = Max__(self->vt[AA], self->vt[GA]);
      self->vt[GA] = 0;
    }
    vol = ((uint8_t *)(int32_t)" %,3;EP^m\200")[self->volume];
    self->vt[GAZ] = Shr__((self->vt[GA] * vol), 7);
    self->vt[AAZ] = Shr__((self->vt[AA] * vol), 7);
    self->vt[FAZ] = Shr__((self->vt[FA] * vol), 7);
    self->vt[NAZ] = Shr__((self->vt[NA] * vol), 7);
    VocalTract_go(&self->v, (Max__((((Min__(pre, self->glide)) * self->dilate) / 100), 1)));
    self->vt[GP] = self->vt[GPS];
    VocalTract_go(&self->v, (Max__(((time * self->dilate) / 100), 1)));
  }
  self->glide = post;
  self->vt[FA] = 0;
  self->vt[GA] = 40;
  return 0;
}

void talk_setVolume(talk *self, int level)
{
  if(level < 0) level = 0;
  if(level > 7) level = 7;
  VocalTract_set_attenuation(&self->v, 7 - level);
}  
