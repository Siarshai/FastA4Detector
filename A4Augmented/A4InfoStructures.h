#ifndef A4INFOSTRUCTURES
#define A4INFOSTRUCTURES

//For black debug majjicks
#define DEBUG_LINES_INFORMATION 1


struct A4PreDetectedRecord {
	CvPoint ulpt;
	CvPoint drpt;
	CvPoint ulptBorder;
	CvPoint drptBorder;
	int widthBorderBlock, heightBorderBlock;
	int safetyWidthMargin, safetyHeightMargin;
	A4PreDetectedRecord(CvPoint aulpt, CvPoint adrpt, CvPoint aulptBorder, CvPoint adrptBorder,
		int awidthBorderBlock, int aheightBorderBlock, int asafetyWidthMargin, int asafetyHeightMargin) : 
		ulpt(aulpt), drpt(adrpt), ulptBorder(aulptBorder), drptBorder(adrptBorder), 
		widthBorderBlock(awidthBorderBlock), heightBorderBlock(aheightBorderBlock), safetyWidthMargin(asafetyWidthMargin), safetyHeightMargin(asafetyHeightMargin) {}
};

struct A4PreciseDetectedRecord {
	CvPoint UL, UR, DL, DR;
	#ifdef DEBUG_LINES_INFORMATION
		CvPoint lineHorUL, lineHorUR, lineHorDL, lineHorDR, lineVerUL, lineVerDL, lineVerUR, lineVerDR;
	#endif DEBUG_LINES_INFORMATION
	A4PreciseDetectedRecord(CvPoint aUL, CvPoint aUR, CvPoint aDL, CvPoint aDR
	#ifdef DEBUG_LINES_INFORMATION
		,
		CvPoint alineHorUL, CvPoint alineHorUR, CvPoint alineHorDL, CvPoint alineHorDR, 
		CvPoint alineVerUL, CvPoint alineVerDL, CvPoint alineVerUR, CvPoint alineVerDR
	#endif DEBUG_LINES_INFORMATION
		): 
			UL(aUL), UR(aUR), DL(aDL), DR(aDR)
	#ifdef DEBUG_LINES_INFORMATION 
				,
			lineHorUL(alineHorUL), lineHorUR(alineHorUR), lineHorDL(alineHorDL), lineHorDR(alineHorDR),
			lineVerUL(alineVerUL), lineVerDL(alineVerDL), lineVerUR(alineVerUR), lineVerDR(alineVerDR)
	#endif DEBUG_LINES_INFORMATION 
			{}
};


#endif //A4INFOSTRUCTURES