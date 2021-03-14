#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fftw3.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/ext/worker/worker.h"
#include "sndfile.h"

/**********************************************************************************************************************************************************/
#define REAL 0
#define IMAG 1

#define SIZE 512
#define PI 3.14159265359

//plugin URI
#define PLUGIN_URI "http://VeJaPlugins.com/plugins/Release/veja-1960-cab"

//macro for Volume in DB to a coefficient
#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

typedef enum {IN, OUT, ATTENUATE, MODEL,MICPOSITION, DISTANCE}PortIndex;

/**********************************************************************************************************************************************************/

/**********************************************************************************************************************************************************/ 
//AKG_C414B_ULS

const char* AKG_Cap__5  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-0.5in.wav";
const char* AKG_Cap_00  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-0in.wav";
const char* AKG_Cap_01  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-1in.wav";
const char* AKG_Cap_02  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-2in.wav";
const char* AKG_Cap_03  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-3in.wav";
const char* AKG_Cap_04  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-4in.wav";
const char* AKG_Cap_05  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-5in.wav";
const char* AKG_Cap_06  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-6in.wav";
const char* AKG_Cap_12  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cap-12in.wav";
const char* AKG_CapEdge__5  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-0.5in.wav";
const char* AKG_CapEdge_00  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-0in.wav";
const char* AKG_CapEdge_01  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-1in.wav";
const char* AKG_CapEdge_02  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-2in.wav";
const char* AKG_CapEdge_03  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-3in.wav";
const char* AKG_CapEdge_04  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-4in.wav";
const char* AKG_CapEdge_05  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-5in.wav";
const char* AKG_CapEdge_06  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-6in.wav";
const char* AKG_CapEdge_12  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-CapEdge-12in.wav";
const char* AKG_Cone__5  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-0.5in.wav";
const char* AKG_Cone_00  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-0in.wav";
const char* AKG_Cone_01  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-1in.wav";
const char* AKG_Cone_02  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-2in.wav";
const char* AKG_Cone_03  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-3in.wav";
const char* AKG_Cone_04  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-4in.wav";
const char* AKG_Cone_05  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-5in.wav";
const char* AKG_Cone_06  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-6in.wav";
const char* AKG_Cone_12  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-Cone-12in.wav";
const char* AKG_ConeEdge__5  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-0.5in.wav";
const char* AKG_ConeEdge_00  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-0in.wav";
const char* AKG_ConeEdge_01  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-1in.wav";
const char* AKG_ConeEdge_02  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-2in.wav";
const char* AKG_ConeEdge_03  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-3in.wav";
const char* AKG_ConeEdge_04  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-4in.wav";
const char* AKG_ConeEdge_05  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-5in.wav";
const char* AKG_ConeEdge_06  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-6in.wav";
const char* AKG_ConeEdge_12  = "/root/.lv2/veja-1960-cab.lv2/IR/AKG_C414B_ULS/Marshall1960A-G12Ms-C414-ConeEdge-12in.wav";

//ElectroVoice RE20

const char* EVOICE_Cap__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-0.5in.wav";
const char* EVOICE_Cap_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-0in.wav";
const char* EVOICE_Cap_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-1in.wav";
const char* EVOICE_Cap_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-2in.wav";
const char* EVOICE_Cap_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-3in.wav";
const char* EVOICE_Cap_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-4in.wav";
const char* EVOICE_Cap_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-5in.wav";
const char* EVOICE_Cap_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-6in.wav";
const char* EVOICE_Cap_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cap-12in.wav";
const char* EVOICE_CapOffAxis__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-0.5in.wav";
const char* EVOICE_CapOffAxis_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-0in.wav";
const char* EVOICE_CapOffAxis_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-1in.wav";
const char* EVOICE_CapOffAxis_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-2in.wav";
const char* EVOICE_CapOffAxis_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-3in.wav";
const char* EVOICE_CapOffAxis_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-4in.wav";
const char* EVOICE_CapOffAxis_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-5in.wav";
const char* EVOICE_CapOffAxis_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-6in.wav";
const char* EVOICE_CapOffAxis_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapOffAxis-12in.wav";
const char* EVOICE_CapEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-0.5in.wav";
const char* EVOICE_CapEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-0in.wav";
const char* EVOICE_CapEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-1in.wav";
const char* EVOICE_CapEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-2in.wav";
const char* EVOICE_CapEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-3in.wav";
const char* EVOICE_CapEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-4in.wav";
const char* EVOICE_CapEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-5in.wav";
const char* EVOICE_CapEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-6in.wav";
const char* EVOICE_CapEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdge-12in.wav";
const char* EVOICE_CapEdgeOffAxis__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-0.5in.wav";
const char* EVOICE_CapEdgeOffAxis_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-0in.wav";
const char* EVOICE_CapEdgeOffAxis_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-1in.wav";
const char* EVOICE_CapEdgeOffAxis_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-2in.wav";
const char* EVOICE_CapEdgeOffAxis_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-3in.wav";
const char* EVOICE_CapEdgeOffAxis_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-4in.wav";
const char* EVOICE_CapEdgeOffAxis_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-5in.wav";
const char* EVOICE_CapEdgeOffAxis_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-6in.wav";
const char* EVOICE_CapEdgeOffAxis_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-CapEdgeOffAxis-12in.wav";
const char* EVOICE_Cone__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-0.5in.wav";
const char* EVOICE_Cone_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-0in.wav";
const char* EVOICE_Cone_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-1in.wav";
const char* EVOICE_Cone_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-2in.wav";
const char* EVOICE_Cone_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-3in.wav";
const char* EVOICE_Cone_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-4in.wav";
const char* EVOICE_Cone_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-5in.wav";
const char* EVOICE_Cone_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-6in.wav";
const char* EVOICE_Cone_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-Cone-12in.wav";
const char* EVOICE_ConeEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-0.5in.wav";
const char* EVOICE_ConeEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-0in.wav";
const char* EVOICE_ConeEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-1in.wav";
const char* EVOICE_ConeEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-2in.wav";
const char* EVOICE_ConeEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-3in.wav";
const char* EVOICE_ConeEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-4in.wav";
const char* EVOICE_ConeEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-5in.wav";
const char* EVOICE_ConeEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-6in.wav";
const char* EVOICE_ConeEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Electrovoice_RE20/Marshall1960A-G12Ms-RE20-ConeEdge-12in.wav";

//Neumann KM84

const char* KM84_Cap__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-0.5in.wav";
const char* KM84_Cap_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-0in.wav";
const char* KM84_Cap_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-1in.wav";
const char* KM84_Cap_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-2in.wav";
const char* KM84_Cap_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-3in.wav";
const char* KM84_Cap_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-4in.wav";
const char* KM84_Cap_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-5in.wav";
const char* KM84_Cap_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-6in.wav";
const char* KM84_Cap_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cap-12in.wav";
const char* KM84_CapEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-0.5in.wav";
const char* KM84_CapEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-0in.wav";
const char* KM84_CapEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-1in.wav";
const char* KM84_CapEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-2in.wav";
const char* KM84_CapEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-3in.wav";
const char* KM84_CapEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-4in.wav";
const char* KM84_CapEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-5in.wav";
const char* KM84_CapEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-6in.wav";
const char* KM84_CapEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-CapEdge-12in.wav";
const char* KM84_Cone__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-0.5in.wav";
const char* KM84_Cone_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-0in.wav";
const char* KM84_Cone_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-1in.wav";
const char* KM84_Cone_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-2in.wav";
const char* KM84_Cone_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-3in.wav";
const char* KM84_Cone_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-4in.wav";
const char* KM84_Cone_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-5in.wav";
const char* KM84_Cone_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-6in.wav";
const char* KM84_Cone_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-Cone-12in.wav";
const char* KM84_ConeEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-0.5in.wav";
const char* KM84_ConeEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-0in.wav";
const char* KM84_ConeEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-1in.wav";
const char* KM84_ConeEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-2in.wav";
const char* KM84_ConeEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-3in.wav";
const char* KM84_ConeEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-4in.wav";
const char* KM84_ConeEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-5in.wav";
const char* KM84_ConeEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-6in.wav";
const char* KM84_ConeEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_KM84/Marshall1960A-G12Ms-KM84-ConeEdge-12in.wav";

//Neumann U87

const char* U87_Cap__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-0.5in.wav";
const char* U87_Cap_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-0in.wav";
const char* U87_Cap_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-1in.wav";
const char* U87_Cap_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-2in.wav";
const char* U87_Cap_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-3in.wav";
const char* U87_Cap_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-4in.wav";
const char* U87_Cap_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-5in.wav";
const char* U87_Cap_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-6in.wav";
const char* U87_Cap_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cap-12in.wav";
const char* U87_CapEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-0.5in.wav";
const char* U87_CapEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-0in.wav";
const char* U87_CapEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-1in.wav";
const char* U87_CapEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-2in.wav";
const char* U87_CapEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-3in.wav";
const char* U87_CapEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-4in.wav";
const char* U87_CapEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-5in.wav";
const char* U87_CapEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-6in.wav";
const char* U87_CapEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-CapEdge-12in.wav";
const char* U87_Cone__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-0.5in.wav";
const char* U87_Cone_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-0in.wav";
const char* U87_Cone_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-1in.wav";
const char* U87_Cone_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-2in.wav";
const char* U87_Cone_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-3in.wav";
const char* U87_Cone_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-4in.wav";
const char* U87_Cone_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-5in.wav";
const char* U87_Cone_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-6in.wav";
const char* U87_Cone_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-Cone-12in.wav";
const char* U87_ConeEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-0.5in.wav";
const char* U87_ConeEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-0in.wav";
const char* U87_ConeEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-1in.wav";
const char* U87_ConeEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-2in.wav";
const char* U87_ConeEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-3in.wav";
const char* U87_ConeEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-4in.wav";
const char* U87_ConeEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-5in.wav";
const char* U87_ConeEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-6in.wav";
const char* U87_ConeEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Neumann_U87/Marshall1960A-G12Ms-U87-ConeEdge-12in.wav";

//ROYER R121

const char* R121_Cap__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-0.5in.wav";
const char* R121_Cap_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-0in.wav";
const char* R121_Cap_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-1in.wav";
const char* R121_Cap_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-2in.wav";
const char* R121_Cap_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-3in.wav";
const char* R121_Cap_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-4in.wav";
const char* R121_Cap_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-5in.wav";
const char* R121_Cap_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-6in.wav";
const char* R121_Cap_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cap-12in.wav";

const char* R121_CapOffAxis__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-0.5in.wav";
const char* R121_CapOffAxis_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-0in.wav";
const char* R121_CapOffAxis_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-1in.wav";
const char* R121_CapOffAxis_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-2in.wav";
const char* R121_CapOffAxis_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-3in.wav";
const char* R121_CapOffAxis_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-4in.wav";
const char* R121_CapOffAxis_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-5in.wav";
const char* R121_CapOffAxis_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-6in.wav";
const char* R121_CapOffAxis_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapOffAxis-12in.wav";

const char* R121_CapEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-0.5in.wav";
const char* R121_CapEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-0in.wav";
const char* R121_CapEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-1in.wav";
const char* R121_CapEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-2in.wav";
const char* R121_CapEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-3in.wav";
const char* R121_CapEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-4in.wav";
const char* R121_CapEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-5in.wav";
const char* R121_CapEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-6in.wav";
const char* R121_CapEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdge-12in.wav";

const char* R121_CapEdgeOffAxis__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-0.5in.wav";
const char* R121_CapEdgeOffAxis_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-0in.wav";
const char* R121_CapEdgeOffAxis_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-1in.wav";
const char* R121_CapEdgeOffAxis_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-2in.wav";
const char* R121_CapEdgeOffAxis_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-3in.wav";
const char* R121_CapEdgeOffAxis_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-4in.wav";
const char* R121_CapEdgeOffAxis_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-5in.wav";
const char* R121_CapEdgeOffAxis_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-6in.wav";
const char* R121_CapEdgeOffAxis_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-CapEdgeOffAxis-12in.wav";

const char* R121_Cone__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-0.5in.wav";
const char* R121_Cone_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-0in.wav";
const char* R121_Cone_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-1in.wav";
const char* R121_Cone_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-2in.wav";
const char* R121_Cone_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-3in.wav";
const char* R121_Cone_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-4in.wav";
const char* R121_Cone_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-5in.wav";
const char* R121_Cone_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-6in.wav";
const char* R121_Cone_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-Cone-12in.wav";

const char* R121_ConeEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-0.5in.wav";
const char* R121_ConeEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-0in.wav";
const char* R121_ConeEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-1in.wav";
const char* R121_ConeEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-2in.wav";
const char* R121_ConeEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-3in.wav";
const char* R121_ConeEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-4in.wav";
const char* R121_ConeEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-5in.wav";
const char* R121_ConeEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-6in.wav";
const char* R121_ConeEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Royer_R121/Marshall1960A-G12Ms-R121-ConeEdge-12in.wav";

//SM_57

const char* SM57_Cap_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-0in.wav";
const char* SM57_Cap__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-0.5in.wav";
const char* SM57_Cap_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-1in.wav";
const char* SM57_Cap_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-2in.wav";
const char* SM57_Cap_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-3in.wav";
const char* SM57_Cap_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-4in.wav";
const char* SM57_Cap_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-5in.wav";
const char* SM57_Cap_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-6in.wav";
const char* SM57_Cap_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cap-12in.wav";

const char* SM57_CapOffAxis__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-0.5in.wav";
const char* SM57_CapOffAxis_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-0in.wav";
const char* SM57_CapOffAxis_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-1in.wav";
const char* SM57_CapOffAxis_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-2in.wav";
const char* SM57_CapOffAxis_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-3in.wav";
const char* SM57_CapOffAxis_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-4in.wav";
const char* SM57_CapOffAxis_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-5in.wav";
const char* SM57_CapOffAxis_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-6in.wav";
const char* SM57_CapOffAxis_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapOffAxis-12in.wav";

const char* SM57_CapEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-0.5in.wav";
const char* SM57_CapEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-0in.wav";
const char* SM57_CapEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-1in.wav";
const char* SM57_CapEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-2in.wav";
const char* SM57_CapEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-3in.wav";
const char* SM57_CapEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-4in.wav";
const char* SM57_CapEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-5in.wav";
const char* SM57_CapEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-6in.wav";
const char* SM57_CapEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdge-12in.wav";

const char* SM57_CapEdgeOffAxis__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-0.5in.wav";
const char* SM57_CapEdgeOffAxis_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-0in.wav";
const char* SM57_CapEdgeOffAxis_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-1in.wav";
const char* SM57_CapEdgeOffAxis_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-2in.wav";
const char* SM57_CapEdgeOffAxis_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-3in.wav";
const char* SM57_CapEdgeOffAxis_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-4in.wav";
const char* SM57_CapEdgeOffAxis_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-5in.wav";
const char* SM57_CapEdgeOffAxis_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-6in.wav";
const char* SM57_CapEdgeOffAxis_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-CapEdgeOffAxis-12in.wav";

const char* SM57_Cone__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-0.5in.wav";
const char* SM57_Cone_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-0in.wav";
const char* SM57_Cone_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-1in.wav";
const char* SM57_Cone_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-2in.wav";
const char* SM57_Cone_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-3in.wav";
const char* SM57_Cone_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-4in.wav";
const char* SM57_Cone_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-5in.wav";
const char* SM57_Cone_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-6in.wav";
const char* SM57_Cone_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-Cone-12in.wav";

const char* SM57_ConeEdge__5 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-0.5in.wav";
const char* SM57_ConeEdge_00 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-0in.wav";
const char* SM57_ConeEdge_01 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-1in.wav";
const char* SM57_ConeEdge_02 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-2in.wav";
const char* SM57_ConeEdge_03 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-3in.wav";
const char* SM57_ConeEdge_04 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-4in.wav";
const char* SM57_ConeEdge_05 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-5in.wav";
const char* SM57_ConeEdge_06 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-6in.wav";
const char* SM57_ConeEdge_12 = "/root/.lv2/veja-1960-cab.lv2/IR/Shure_SM57/Marshall1960A-G12Ms-SM57-ConeEdge-12in.wav";

static const char* samplePath[9][6][6];

typedef struct {
        SF_INFO  info;      // Info about sample from sndfile
        float*   data;      // Sample data in float
        char*    path;      // Path of file
        uint32_t path_len;  // Length of path
} XXX;

typedef struct{
    float const *in;
    float *out;
    float *model;
    float *Distance;
    float *outbuf;
    float *inbuf;
    float *IR;
    float *impulseResponse;
    float *overlap;
    float *oA;
    float *oB;
    float *oC;
    float *micPos;
    int modelPrevious;
    int prevMicPos;
    int prevDistance;
    const float *attenuation;

    fftwf_complex *outComplex;
    fftwf_complex *IRout;
    fftwf_complex *convolved;

    fftwf_plan fft;
    fftwf_plan ifft;
    fftwf_plan IRfft;

    XXX* xxx;

} Cabsim;

/**********************************************************************************************************************************************************/
//functions

/**********************************************************************************************************************************************************/

static XXX*
load_xxx(Cabsim* self, const char* path)
{
        XXX* const  xxx  = (XXX*)malloc(sizeof(XXX)); 
        
        const size_t path_len = strlen(path);

        SF_INFO* const info    = &xxx->info;
        SNDFILE* const sndfile = sf_open(path, SFM_READ, info);

        if (!sndfile || !info->frames || (info->channels != 1))
        {
            free(xxx);
            return NULL;
        }

        // Read data
        float* const data = malloc(sizeof(float) * info->frames);
        if (!data)
        {
            return NULL;
        }
        sf_seek(sndfile, 0ul, SEEK_SET);
        sf_read_float(sndfile, data, info->frames);
        sf_close(sndfile);

        // Fill xxx struct and return it
        xxx->data     = data;
        xxx->path     = (char*)malloc(path_len + 1);
        xxx->path_len = (uint32_t)path_len;
        memcpy(xxx->path, path, path_len + 1);

        return xxx;
}

static void
fill_samplePath()
{
    samplePath[0][0][0] = AKG_Cap__5;
    samplePath[1][0][0] = AKG_Cap_00;
    samplePath[2][0][0] = AKG_Cap_01;
    samplePath[3][0][0] = AKG_Cap_02;
    samplePath[4][0][0] = AKG_Cap_03;
    samplePath[5][0][0] = AKG_Cap_04;
    samplePath[6][0][0] = AKG_Cap_05;
    samplePath[7][0][0] = AKG_Cap_06;
    samplePath[8][0][0] = AKG_Cap_12;
    samplePath[0][1][0] = AKG_Cap__5;
    samplePath[1][1][0] = AKG_Cap_00;
    samplePath[2][1][0] = AKG_Cap_01;
    samplePath[3][1][0] = AKG_Cap_02;
    samplePath[4][1][0] = AKG_Cap_03;
    samplePath[5][1][0] = AKG_Cap_04;
    samplePath[6][1][0] = AKG_Cap_05;
    samplePath[7][1][0] = AKG_Cap_06;
    samplePath[8][1][0] = AKG_Cap_12;
    samplePath[0][2][0] = AKG_CapEdge__5;
    samplePath[1][2][0] = AKG_CapEdge_00;
    samplePath[2][2][0] = AKG_CapEdge_01;
    samplePath[3][2][0] = AKG_CapEdge_02;
    samplePath[4][2][0] = AKG_CapEdge_03;
    samplePath[5][2][0] = AKG_CapEdge_04;
    samplePath[6][2][0] = AKG_CapEdge_05;
    samplePath[7][2][0] = AKG_CapEdge_06;
    samplePath[8][2][0] = AKG_CapEdge_12;
    samplePath[0][3][0] = AKG_CapEdge__5;
    samplePath[1][3][0] = AKG_CapEdge_00;
    samplePath[2][3][0] = AKG_CapEdge_01;
    samplePath[3][3][0] = AKG_CapEdge_02;
    samplePath[4][3][0] = AKG_CapEdge_03;
    samplePath[5][3][0] = AKG_CapEdge_04;
    samplePath[6][3][0] = AKG_CapEdge_05;
    samplePath[7][3][0] = AKG_CapEdge_06;
    samplePath[8][3][0] = AKG_CapEdge_12;
    samplePath[0][4][0] = AKG_Cone__5;
    samplePath[1][4][0] = AKG_Cone_00;
    samplePath[2][4][0] = AKG_Cone_01;
    samplePath[3][4][0] = AKG_Cone_02;
    samplePath[4][4][0] = AKG_Cone_03;
    samplePath[5][4][0] = AKG_Cone_04;
    samplePath[6][4][0] = AKG_Cone_05;
    samplePath[7][4][0] = AKG_Cone_06;
    samplePath[8][4][0] = AKG_Cone_12;
    samplePath[0][5][0] = AKG_ConeEdge__5;
    samplePath[1][5][0] = AKG_ConeEdge_00;
    samplePath[2][5][0] = AKG_ConeEdge_01;
    samplePath[3][5][0] = AKG_ConeEdge_02;
    samplePath[4][5][0] = AKG_ConeEdge_03;
    samplePath[5][5][0] = AKG_ConeEdge_04;
    samplePath[6][5][0] = AKG_ConeEdge_05;
    samplePath[7][5][0] = AKG_ConeEdge_06;
    samplePath[8][5][0] = AKG_ConeEdge_12;

    samplePath[0][0][1] = EVOICE_Cap__5;
    samplePath[1][0][1] = EVOICE_Cap_00;
    samplePath[2][0][1] = EVOICE_Cap_01;
    samplePath[3][0][1] = EVOICE_Cap_02;
    samplePath[4][0][1] = EVOICE_Cap_03;
    samplePath[5][0][1] = EVOICE_Cap_04;
    samplePath[6][0][1] = EVOICE_Cap_05;
    samplePath[7][0][1] = EVOICE_Cap_06;
    samplePath[8][0][1] = EVOICE_Cap_12;
    samplePath[0][1][1] = EVOICE_CapOffAxis__5;
    samplePath[1][1][1] = EVOICE_CapOffAxis_00;
    samplePath[2][1][1] = EVOICE_CapOffAxis_01;
    samplePath[3][1][1] = EVOICE_CapOffAxis_02;
    samplePath[4][1][1] = EVOICE_CapOffAxis_03;
    samplePath[5][1][1] = EVOICE_CapOffAxis_04;
    samplePath[6][1][1] = EVOICE_CapOffAxis_05;
    samplePath[7][1][1] = EVOICE_CapOffAxis_06;
    samplePath[8][1][1] = EVOICE_CapOffAxis_12;
    samplePath[0][2][1] = EVOICE_CapEdge__5;
    samplePath[1][2][1] = EVOICE_CapEdge_00;
    samplePath[2][2][1] = EVOICE_CapEdge_01;
    samplePath[3][2][1] = EVOICE_CapEdge_02;
    samplePath[4][2][1] = EVOICE_CapEdge_03;
    samplePath[5][2][1] = EVOICE_CapEdge_04;
    samplePath[6][2][1] = EVOICE_CapEdge_05;
    samplePath[7][2][1] = EVOICE_CapEdge_06;
    samplePath[8][2][1] = EVOICE_CapEdge_12;
    samplePath[0][3][1] = EVOICE_CapEdgeOffAxis__5;
    samplePath[1][3][1] = EVOICE_CapEdgeOffAxis_00;
    samplePath[2][3][1] = EVOICE_CapEdgeOffAxis_01;
    samplePath[3][3][1] = EVOICE_CapEdgeOffAxis_02;
    samplePath[4][3][1] = EVOICE_CapEdgeOffAxis_03;
    samplePath[5][3][1] = EVOICE_CapEdgeOffAxis_04;
    samplePath[6][3][1] = EVOICE_CapEdgeOffAxis_05;
    samplePath[7][3][1] = EVOICE_CapEdgeOffAxis_06;
    samplePath[8][3][1] = EVOICE_CapEdgeOffAxis_12;
    samplePath[0][4][1] = EVOICE_Cone__5;
    samplePath[1][4][1] = EVOICE_Cone_00;
    samplePath[2][4][1] = EVOICE_Cone_01;
    samplePath[3][4][1] = EVOICE_Cone_02;
    samplePath[4][4][1] = EVOICE_Cone_03;
    samplePath[5][4][1] = EVOICE_Cone_04;
    samplePath[6][4][1] = EVOICE_Cone_05;
    samplePath[7][4][1] = EVOICE_Cone_06;
    samplePath[8][4][1] = EVOICE_Cone_12;
    samplePath[0][5][1] = EVOICE_ConeEdge__5;
    samplePath[1][5][1] = EVOICE_ConeEdge_00;
    samplePath[2][5][1] = EVOICE_ConeEdge_01;
    samplePath[3][5][1] = EVOICE_ConeEdge_02;
    samplePath[4][5][1] = EVOICE_ConeEdge_03;
    samplePath[5][5][1] = EVOICE_ConeEdge_04;
    samplePath[6][5][1] = EVOICE_ConeEdge_05;
    samplePath[7][5][1] = EVOICE_ConeEdge_06;
    samplePath[8][5][1] = EVOICE_ConeEdge_12;

    samplePath[0][0][2] = KM84_Cap__5;
    samplePath[1][0][2] = KM84_Cap_00;
    samplePath[2][0][2] = KM84_Cap_01;
    samplePath[3][0][2] = KM84_Cap_02;
    samplePath[4][0][2] = KM84_Cap_03;
    samplePath[5][0][2] = KM84_Cap_04;
    samplePath[6][0][2] = KM84_Cap_05;
    samplePath[7][0][2] = KM84_Cap_06;
    samplePath[8][0][2] = KM84_Cap_12;
    samplePath[0][1][2] = KM84_Cap__5;
    samplePath[1][1][2] = KM84_Cap_00;
    samplePath[2][1][2] = KM84_Cap_01;
    samplePath[3][1][2] = KM84_Cap_02;
    samplePath[4][1][2] = KM84_Cap_03;
    samplePath[5][1][2] = KM84_Cap_04;
    samplePath[6][1][2] = KM84_Cap_05;
    samplePath[7][1][2] = KM84_Cap_06;
    samplePath[8][1][2] = KM84_Cap_12;
    samplePath[0][2][2] = KM84_CapEdge__5;
    samplePath[1][2][2] = KM84_CapEdge_00;
    samplePath[2][2][2] = KM84_CapEdge_01;
    samplePath[3][2][2] = KM84_CapEdge_02;
    samplePath[4][2][2] = KM84_CapEdge_03;
    samplePath[5][2][2] = KM84_CapEdge_04;
    samplePath[6][2][2] = KM84_CapEdge_05;
    samplePath[7][2][2] = KM84_CapEdge_06;
    samplePath[8][2][2] = KM84_CapEdge_12;
    samplePath[0][3][2] = KM84_CapEdge__5;
    samplePath[1][3][2] = KM84_CapEdge_00;
    samplePath[2][3][2] = KM84_CapEdge_01;
    samplePath[3][3][2] = KM84_CapEdge_02;
    samplePath[4][3][2] = KM84_CapEdge_03;
    samplePath[5][3][2] = KM84_CapEdge_04;
    samplePath[6][3][2] = KM84_CapEdge_05;
    samplePath[7][3][2] = KM84_CapEdge_06;
    samplePath[8][3][2] = KM84_CapEdge_12;
    samplePath[0][4][2] = KM84_Cone__5;
    samplePath[1][4][2] = KM84_Cone_00;
    samplePath[2][4][2] = KM84_Cone_01;
    samplePath[3][4][2] = KM84_Cone_02;
    samplePath[4][4][2] = KM84_Cone_03;
    samplePath[5][4][2] = KM84_Cone_04;
    samplePath[6][4][2] = KM84_Cone_05;
    samplePath[7][4][2] = KM84_Cone_06;
    samplePath[8][4][2] = KM84_Cone_12;
    samplePath[0][5][2] = KM84_ConeEdge__5;
    samplePath[1][5][2] = KM84_ConeEdge_00;
    samplePath[2][5][2] = KM84_ConeEdge_01;
    samplePath[3][5][2] = KM84_ConeEdge_02;
    samplePath[4][5][2] = KM84_ConeEdge_03;
    samplePath[5][5][2] = KM84_ConeEdge_04;
    samplePath[6][5][2] = KM84_ConeEdge_05;
    samplePath[7][5][2] = KM84_ConeEdge_06;
    samplePath[8][5][2] = KM84_ConeEdge_12;

    samplePath[0][0][3] = U87_Cap__5;
    samplePath[1][0][3] = U87_Cap_00;
    samplePath[2][0][3] = U87_Cap_01;
    samplePath[3][0][3] = U87_Cap_02;
    samplePath[4][0][3] = U87_Cap_03;
    samplePath[5][0][3] = U87_Cap_04;
    samplePath[6][0][3] = U87_Cap_05;
    samplePath[7][0][3] = U87_Cap_06;
    samplePath[8][0][3] = U87_Cap_12;
    samplePath[0][1][3] = U87_Cap__5;
    samplePath[1][1][3] = U87_Cap_00;
    samplePath[2][1][3] = U87_Cap_01;
    samplePath[3][1][3] = U87_Cap_02;
    samplePath[4][1][3] = U87_Cap_03;
    samplePath[5][1][3] = U87_Cap_04;
    samplePath[6][1][3] = U87_Cap_05;
    samplePath[7][1][3] = U87_Cap_06;
    samplePath[8][1][3] = U87_Cap_12;
    samplePath[0][2][3] = U87_CapEdge__5;
    samplePath[1][2][3] = U87_CapEdge_00;
    samplePath[2][2][3] = U87_CapEdge_01;
    samplePath[3][2][3] = U87_CapEdge_02;
    samplePath[4][2][3] = U87_CapEdge_03;
    samplePath[5][2][3] = U87_CapEdge_04;
    samplePath[6][2][3] = U87_CapEdge_05;
    samplePath[7][2][3] = U87_CapEdge_06;
    samplePath[8][2][3] = U87_CapEdge_12;
    samplePath[0][3][3] = U87_CapEdge__5;
    samplePath[1][3][3] = U87_CapEdge_00;
    samplePath[2][3][3] = U87_CapEdge_01;
    samplePath[3][3][3] = U87_CapEdge_02;
    samplePath[4][3][3] = U87_CapEdge_03;
    samplePath[5][3][3] = U87_CapEdge_04;
    samplePath[6][3][3] = U87_CapEdge_05;
    samplePath[7][3][3] = U87_CapEdge_06;
    samplePath[8][3][3] = U87_CapEdge_12;
    samplePath[0][4][3] = U87_Cone__5;
    samplePath[1][4][3] = U87_Cone_00;
    samplePath[2][4][3] = U87_Cone_01;
    samplePath[3][4][3] = U87_Cone_02;
    samplePath[4][4][3] = U87_Cone_03;
    samplePath[5][4][3] = U87_Cone_04;
    samplePath[6][4][3] = U87_Cone_05;
    samplePath[7][4][3] = U87_Cone_06;
    samplePath[8][4][3] = U87_Cone_12;
    samplePath[0][5][3] = U87_ConeEdge__5;
    samplePath[1][5][3] = U87_ConeEdge_00;
    samplePath[2][5][3] = U87_ConeEdge_01;
    samplePath[3][5][3] = U87_ConeEdge_02;
    samplePath[4][5][3] = U87_ConeEdge_03;
    samplePath[5][5][3] = U87_ConeEdge_04;
    samplePath[6][5][3] = U87_ConeEdge_05;
    samplePath[7][5][3] = U87_ConeEdge_06;
    samplePath[8][5][3] = U87_ConeEdge_12;

    samplePath[0][0][4] = R121_Cap__5;
    samplePath[1][0][4] = R121_Cap_00;
    samplePath[2][0][4] = R121_Cap_01;
    samplePath[3][0][4] = R121_Cap_02;
    samplePath[4][0][4] = R121_Cap_03;
    samplePath[5][0][4] = R121_Cap_04;
    samplePath[6][0][4] = R121_Cap_05;
    samplePath[7][0][4] = R121_Cap_06;
    samplePath[8][0][4] = R121_Cap_12;
    samplePath[0][1][4] = R121_CapOffAxis__5;
    samplePath[1][1][4] = R121_CapOffAxis_00;
    samplePath[2][1][4] = R121_CapOffAxis_01;
    samplePath[3][1][4] = R121_CapOffAxis_02;
    samplePath[4][1][4] = R121_CapOffAxis_03;
    samplePath[5][1][4] = R121_CapOffAxis_04;
    samplePath[6][1][4] = R121_CapOffAxis_05;
    samplePath[7][1][4] = R121_CapOffAxis_06;
    samplePath[8][1][4] = R121_CapOffAxis_12;
    samplePath[0][2][4] = R121_CapEdge__5;
    samplePath[1][2][4] = R121_CapEdge_00;
    samplePath[2][2][4] = R121_CapEdge_01;
    samplePath[3][2][4] = R121_CapEdge_02;
    samplePath[4][2][4] = R121_CapEdge_03;
    samplePath[5][2][4] = R121_CapEdge_04;
    samplePath[6][2][4] = R121_CapEdge_05;
    samplePath[7][2][4] = R121_CapEdge_06;
    samplePath[8][2][4] = R121_CapEdge_12;
    samplePath[0][3][4] = R121_CapEdgeOffAxis__5;
    samplePath[1][3][4] = R121_CapEdgeOffAxis_00;
    samplePath[2][3][4] = R121_CapEdgeOffAxis_01;
    samplePath[3][3][4] = R121_CapEdgeOffAxis_02;
    samplePath[4][3][4] = R121_CapEdgeOffAxis_03;
    samplePath[5][3][4] = R121_CapEdgeOffAxis_04;
    samplePath[6][3][4] = R121_CapEdgeOffAxis_05;
    samplePath[7][3][4] = R121_CapEdgeOffAxis_06;
    samplePath[8][3][4] = R121_CapEdgeOffAxis_12;
    samplePath[0][4][4] = R121_Cone__5;
    samplePath[1][4][4] = R121_Cone_00;
    samplePath[2][4][4] = R121_Cone_01;
    samplePath[3][4][4] = R121_Cone_02;
    samplePath[4][4][4] = R121_Cone_03;
    samplePath[5][4][4] = R121_Cone_04;
    samplePath[6][4][4] = R121_Cone_05;
    samplePath[7][4][4] = R121_Cone_06;
    samplePath[8][4][4] = R121_Cone_12;
    samplePath[0][5][4] = R121_ConeEdge__5;
    samplePath[1][5][4] = R121_ConeEdge_00;
    samplePath[2][5][4] = R121_ConeEdge_01;
    samplePath[3][5][4] = R121_ConeEdge_02;
    samplePath[4][5][4] = R121_ConeEdge_03;
    samplePath[5][5][4] = R121_ConeEdge_04;
    samplePath[6][5][4] = R121_ConeEdge_05;
    samplePath[7][5][4] = R121_ConeEdge_06;
    samplePath[8][5][4] = R121_ConeEdge_12;

    samplePath[0][0][5] = SM57_Cap__5;
    samplePath[1][0][5] = SM57_Cap_00;
    samplePath[2][0][5] = SM57_Cap_01;
    samplePath[3][0][5] = SM57_Cap_02;
    samplePath[4][0][5] = SM57_Cap_03;
    samplePath[5][0][5] = SM57_Cap_04;
    samplePath[6][0][5] = SM57_Cap_05;
    samplePath[7][0][5] = SM57_Cap_06;
    samplePath[8][0][5] = SM57_Cap_12;     
    samplePath[0][1][5] = SM57_CapOffAxis__5;
    samplePath[1][1][5] = SM57_CapOffAxis_00;
    samplePath[2][1][5] = SM57_CapOffAxis_01;
    samplePath[3][1][5] = SM57_CapOffAxis_02;
    samplePath[4][1][5] = SM57_CapOffAxis_03;
    samplePath[5][1][5] = SM57_CapOffAxis_04;
    samplePath[6][1][5] = SM57_CapOffAxis_05;
    samplePath[7][1][5] = SM57_CapOffAxis_06;
    samplePath[8][1][5] = SM57_CapOffAxis_12;
    samplePath[0][2][5] = SM57_CapEdge__5;
    samplePath[1][2][5] = SM57_CapEdge_00;
    samplePath[2][2][5] = SM57_CapEdge_01;
    samplePath[3][2][5] = SM57_CapEdge_02;
    samplePath[4][2][5] = SM57_CapEdge_03;
    samplePath[5][2][5] = SM57_CapEdge_04;
    samplePath[6][2][5] = SM57_CapEdge_05;
    samplePath[7][2][5] = SM57_CapEdge_06;
    samplePath[8][2][5] = SM57_CapEdge_12;
    samplePath[0][3][5] = SM57_CapEdgeOffAxis__5;
    samplePath[1][3][5] = SM57_CapEdgeOffAxis_00;
    samplePath[2][3][5] = SM57_CapEdgeOffAxis_01;
    samplePath[3][3][5] = SM57_CapEdgeOffAxis_02;
    samplePath[4][3][5] = SM57_CapEdgeOffAxis_03;
    samplePath[5][3][5] = SM57_CapEdgeOffAxis_04;
    samplePath[6][3][5] = SM57_CapEdgeOffAxis_05;
    samplePath[7][3][5] = SM57_CapEdgeOffAxis_06;
    samplePath[8][3][5] = SM57_CapEdgeOffAxis_12;
    samplePath[0][4][5] = SM57_Cone__5;
    samplePath[1][4][5] = SM57_Cone_00;
    samplePath[2][4][5] = SM57_Cone_01;
    samplePath[3][4][5] = SM57_Cone_02;
    samplePath[4][4][5] = SM57_Cone_03;
    samplePath[5][4][5] = SM57_Cone_04;
    samplePath[6][4][5] = SM57_Cone_05;
    samplePath[7][4][5] = SM57_Cone_06;
    samplePath[8][4][5] = SM57_Cone_12;
    samplePath[0][5][5] = SM57_ConeEdge__5;
    samplePath[1][5][5] = SM57_ConeEdge_00;
    samplePath[2][5][5] = SM57_ConeEdge_01;
    samplePath[3][5][5] = SM57_ConeEdge_02;
    samplePath[4][5][5] = SM57_ConeEdge_03;
    samplePath[5][5][5] = SM57_ConeEdge_04;
    samplePath[6][5][5] = SM57_ConeEdge_05;
    samplePath[7][5][5] = SM57_ConeEdge_06;
    samplePath[8][5][5] = SM57_ConeEdge_12;
}

static void
free_xxx(Cabsim* self, XXX* xxx)
{
    if (xxx)
    {
        free(xxx->path);
        free(xxx->data);
        free(xxx);
    }
}
/**********************************************************************************************************************************************************/
//end functions

/**********************************************************************************************************************************************************/

static LV2_Handle
instantiate(const LV2_Descriptor*   descriptor,
double                              samplerate,
const char*                         bundle_path,
const LV2_Feature* const* features)
{
    Cabsim* cabsim = (Cabsim*)malloc(sizeof(Cabsim));

    fill_samplePath();

    cabsim->outComplex = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*(SIZE));
    cabsim->IRout =  (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*(SIZE));
    cabsim->convolved =  (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*(SIZE));

    cabsim->impulseResponse = (float *) calloc((SIZE),sizeof(float)); 
    cabsim->overlap = (float *) calloc((SIZE),sizeof(float));
    cabsim->outbuf = (float *) calloc((SIZE),sizeof(float));
    cabsim->inbuf = (float *) calloc((SIZE),sizeof(float));
    cabsim->IR = (float *) calloc((SIZE),sizeof(float));
    cabsim->oA = (float *) calloc((SIZE),sizeof(float));
    cabsim->oB = (float *) calloc((SIZE),sizeof(float));
    cabsim->oC = (float *) calloc((SIZE),sizeof(float));

    const char* wisdomFile = "cabsim.wisdom";
    //open file A
    const size_t path_len    = strlen(bundle_path);
    const size_t file_len    = strlen(wisdomFile);
    const size_t len         = path_len + file_len;
    char*        wisdom_path = (char*)malloc(len + 1);
    snprintf(wisdom_path, len + 1, "%s%s", bundle_path, wisdomFile);

    if (fftwf_import_wisdom_from_filename(wisdom_path) != 0)
    {
        cabsim->fft = fftwf_plan_dft_r2c_1d(SIZE, cabsim->inbuf, cabsim->outComplex, FFTW_WISDOM_ONLY|FFTW_ESTIMATE);
        cabsim->IRfft = fftwf_plan_dft_r2c_1d(SIZE, cabsim->IR, cabsim->IRout, FFTW_WISDOM_ONLY|FFTW_ESTIMATE);
        cabsim->ifft = fftwf_plan_dft_c2r_1d(SIZE, cabsim->convolved, cabsim->outbuf, FFTW_WISDOM_ONLY|FFTW_ESTIMATE);
    } else {
        cabsim->fft = fftwf_plan_dft_r2c_1d(SIZE, cabsim->inbuf, cabsim->outComplex, FFTW_ESTIMATE);
        cabsim->IRfft = fftwf_plan_dft_r2c_1d(SIZE, cabsim->IR, cabsim->IRout, FFTW_ESTIMATE);
        cabsim->ifft = fftwf_plan_dft_c2r_1d(SIZE, cabsim->convolved, cabsim->outbuf, FFTW_ESTIMATE);
    }

    cabsim->xxx = load_xxx(cabsim, samplePath[1][1][4]);

    free(wisdom_path);

    return (LV2_Handle)cabsim;
}
/**********************************************************************************************************************************************************/
static void connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    Cabsim* cabsim = (Cabsim*)instance;

    switch ((PortIndex)port)
    {
        case IN:
            cabsim->in = (float*) data;
            break;
        case OUT:
            cabsim->out = (float*) data;
            break;
        case ATTENUATE:
            cabsim->attenuation = (const float*) data;
            break;
        case MODEL:
            cabsim->model = (float*) data;
            break;
        case MICPOSITION:
            cabsim->micPos = (float*)data;
            break;
        case DISTANCE:
            cabsim->Distance = (float*)data;
            break;
    }
}
/**********************************************************************************************************************************************************/
void activate(LV2_Handle instance)
{
    Cabsim* cabsim = (Cabsim*)instance;
    cabsim->modelPrevious = 9999;
    cabsim->prevMicPos = 9999;
    cabsim->prevDistance = 99;

}

/**********************************************************************************************************************************************************/
void run(LV2_Handle instance, uint32_t n_samples)
{
    Cabsim* cabsim = (Cabsim*)instance;    

    const float const *in = cabsim->in;
    float *out = cabsim->out;
    float *outbuf = cabsim->outbuf;
    float *inbuf = cabsim->inbuf;
    float *IR = cabsim->IR;
    float *overlap = cabsim->overlap;
    float *oA = cabsim->oA;
    float *oB = cabsim->oB;
    float *oC = cabsim->oC;
    int model = (int)(*(cabsim->model));
    int position = (int)(*cabsim->micPos);
    int distance = (int)(*cabsim->Distance);
    const float attenuation = *cabsim->attenuation; 

    const float coef = DB_CO(attenuation);

    uint32_t i, j, m;
    
    int multiplier = (SIZE / n_samples);

    if(n_samples == 128)
    {
        multiplier = 4;
    }
    else if (n_samples == 256)
    {
        multiplier = 2;
    }

    //copy inputbuffer and IR buffer with zero padding.
    if((cabsim->modelPrevious!=model || cabsim->prevMicPos!=position) || cabsim->prevDistance!= distance)
    {
        cabsim->xxx = load_xxx(cabsim,samplePath[distance][position][model]);

        cabsim->modelPrevious = model;
        cabsim->prevDistance = distance;
        cabsim->prevMicPos = position;  //changed micPos to float port. change calculations

    	for ( i = 0; i < n_samples * multiplier; i++)
    	{
        	inbuf[i] = (i < n_samples) ? (in[i] * coef): 0.0f;
            IR[i] = (i < n_samples) ? cabsim->xxx->data[i] : 0.0f;
        }
        fftwf_execute(cabsim->IRfft);
    }	
    else
    {		
    	for ( i = 0; i < n_samples * multiplier; i++)
    	{
        	inbuf[i] = (i < n_samples) ? (in[i] * coef): 0.0f;
        }
    }
    fftwf_execute(cabsim->fft);

    //complex multiplication
    for(m = 0; m < ((n_samples / 2) * multiplier) ;m++)
    {
        //real component
        cabsim->convolved[m][REAL] = cabsim->outComplex[m][REAL] * cabsim->IRout[m][REAL] - cabsim->outComplex[m][IMAG] * cabsim->IRout[m][IMAG];
        //imaginary component
        cabsim->convolved[m][IMAG] = cabsim->outComplex[m][REAL] * cabsim->IRout[m][IMAG] + cabsim->outComplex[m][IMAG] * cabsim->IRout[m][REAL];
    }

    fftwf_execute(cabsim->ifft);

    //normalize output with overlap add.
    if(n_samples == 256)
    {
        for ( j = 0; j < n_samples * multiplier; j++)
        {
            if(j < n_samples)
            {
                out[j] = ((outbuf[j] / (n_samples * multiplier)) + overlap[j]) * 0.25f;
            }
            else
            {
                overlap[j - n_samples] = outbuf[j]  / (n_samples * multiplier);
            }
        }
    }
    else if (n_samples == 128)
    {
        for ( j = 0; j < n_samples * multiplier; j++)
        {
            if(j < n_samples)   //runs 128 times filling the output buffer with overap add
            {
                out[j] = (outbuf[j] / (n_samples * multiplier) + oA[j] + oB[j] + oC[j]) * 0.25f;
            }
            else   
            {
                oC[j - n_samples] = oB[j]; // 128 samples of usefull data
                oB[j - n_samples] = oA[j];  //filled with samples 128 to 255 of usefull data
                oA[j - n_samples] = (outbuf[j] / (n_samples * multiplier)); //filled with 384 samples
            }
        }
    }
}

/**********************************************************************************************************************************************************/
void deactivate(LV2_Handle instance)
{
    // TODO: include the deactivate function code here
}
/**********************************************************************************************************************************************************/
void cleanup(LV2_Handle instance)
{
    Cabsim* cabsim = (Cabsim*)instance;

    fftwf_destroy_plan(cabsim->fft);
    fftwf_destroy_plan(cabsim->ifft);
    fftwf_destroy_plan(cabsim->IRfft);
    //free fft memory
    fftwf_free(cabsim->outComplex);
    fftwf_free(cabsim->IRout);
    fftwf_free(cabsim->convolved);
    //free allocated memory
    free_xxx(cabsim, cabsim->xxx);
    free(instance);
}
/**********************************************************************************************************************************************************/
const void* extension_data(const char* uri)
{
    return NULL;
}
/**********************************************************************************************************************************************************/
static const LV2_Descriptor Descriptor = {
    PLUGIN_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};
/**********************************************************************************************************************************************************/
LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    if (index == 0) return &Descriptor;
    else return NULL;
}
/**********************************************************************************************************************************************************/