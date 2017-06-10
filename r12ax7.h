/*
        dsp/r12ax7.h

        Copyright 2003-5 Tim Goetze <tim@quitte.de>

        http://quitte.de/dsp/

        12AX7 voltage transfer function, sampled from a spice simulation
        running a simple preamp net with a 12ax7 model graciously provided
        by duncanamps.com .

 */
/*
        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        as published by the Free Software Foundation; either version 2
        of the License, or (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
        02111-1307, USA or point your web browser to http://www.gnu.org.
*/

#ifndef _DSP_R_12_AX_7_H_
#define _DSP_R_12_AX_7_H_

namespace r12AX7 {

enum { Zero = 566, Samples = 1668, };

static float v2v[] = {
	0.277279436588, 0.277249097824, 0.277187168598, 0.277105689049,
	0.277008116245, 0.276897609234, 0.276775300503, 0.276642978191,
	0.276501119137, 0.276351094246, 0.276193082333, 0.276027917862,
	0.275855839252, 0.275678634644, 0.275495171547, 0.275305449963,
	0.275109887123, 0.274909138680, 0.274703323841, 0.274492740631,
	0.274277567863, 0.274058103561, 0.273834228516, 0.273606359959,
	0.273374497890, 0.273138821125, 0.272899389267, 0.272656440735,
	0.272409796715, 0.272159934044, 0.271906614304, 0.271650075912,
	0.271390438080, 0.271127700806, 0.270861923695, 0.270593225956,
	0.270321667194, 0.270047247410, 0.269770205021, 0.269490361214,
	0.269207894802, 0.268922865391, 0.268635272980, 0.268345177174,
	0.268052697182, 0.267757713795, 0.267460465431, 0.267160952091,
	0.266858994961, 0.266555011272, 0.266248643398, 0.265940189362,
	0.265629589558, 0.265316963196, 0.265002191067, 0.264685392380,
	0.264366567135, 0.264045774937, 0.263723075390, 0.263398468494,
	0.263071894646, 0.262743532658, 0.262413203716, 0.262081146240,
	0.261747241020, 0.261411607265, 0.261074185371, 0.260735154152,
	0.260394215584, 0.260051786900, 0.259707629681, 0.259361803532,
	0.259014308453, 0.258665204048, 0.258314549923, 0.257962286472,
	0.257608532906, 0.257253229618, 0.256896317005, 0.256537973881,
	0.256178140640, 0.255816757679, 0.255454003811, 0.255089759827,
	0.254724085331, 0.254356980324, 0.253988444805, 0.253618597984,
	0.253247320652, 0.252874732018, 0.252500772476, 0.252125442028,
	0.251748800278, 0.251370847225, 0.250991582870, 0.250611066818,
	0.250229299068, 0.249846160412, 0.249461889267, 0.249076247215,
	0.248689472675, 0.248301327229, 0.247912168503, 0.247521698475,
	0.247129976749, 0.246737182140, 0.246343135834, 0.245947897434,
	0.245551586151, 0.245154082775, 0.244755446911, 0.244355678558,
	0.243954837322, 0.243552863598, 0.243149697781, 0.242745578289,
	0.242340266705, 0.241933882236, 0.241526484489, 0.241117894650,
	0.240708351135, 0.240297734737, 0.239886105061, 0.239473402500,
	0.239059746265, 0.238644957542, 0.238229274750, 0.237812399864,
	0.237394690514, 0.236975908279, 0.236556172371, 0.236135542393,
	0.235713779926, 0.235291123390, 0.234867513180, 0.234442949295,
	0.234017431736, 0.233590960503, 0.233163535595, 0.232735216618,
	0.232305943966, 0.231875777245, 0.231444656849, 0.231012642384,
	0.230579853058, 0.230145990849, 0.229711294174, 0.229275703430,
	0.228839278221, 0.228401958942, 0.227963745594, 0.227524638176,
	0.227084696293, 0.226643919945, 0.226202249527, 0.225759744644,
	0.225316464901, 0.224872291088, 0.224427282810, 0.223981380463,
	0.223534762859, 0.223087251186, 0.222638964653, 0.222189962864,
	0.221740007401, 0.221289336681, 0.220837771893, 0.220385491848,
	0.219932436943, 0.219478607178, 0.219023883343, 0.218568444252,
	0.218112349510, 0.217655301094, 0.217197597027, 0.216739118099,
	0.216279804707, 0.215819895267, 0.215359091759, 0.214897632599,
	0.214435338974, 0.213972389698, 0.213508665562, 0.213044166565,
	0.212579071522, 0.212113201618, 0.211646556854, 0.211179137230,
	0.210711181164, 0.210242390633, 0.209772944450, 0.209302783012,
	0.208831906319, 0.208360373974, 0.207888066769, 0.207415163517,
	0.206941485405, 0.206467092037, 0.205992162228, 0.205516517162,
	0.205040097237, 0.204563021660, 0.204085409641, 0.203607022762,
	0.203128039837, 0.202648282051, 0.202167987823, 0.201686978340,
	0.201205253601, 0.200723052025, 0.200240015984, 0.199756443501,
	0.199272215366, 0.198787331581, 0.198301851749, 0.197815716267,
	0.197328925133, 0.196841537952, 0.196353554726, 0.195864915848,
	0.195375621319, 0.194885730743, 0.194395244122, 0.193904161453,
	0.193412423134, 0.192920088768, 0.192427158356, 0.191933691502,
	0.191439509392, 0.190944850445, 0.190449535847, 0.189953625202,
	0.189457118511, 0.188960015774, 0.188462376595, 0.187964081764,
	0.187465250492, 0.186965823174, 0.186465859413, 0.185965299606,
	0.185464143753, 0.184962451458, 0.184460222721, 0.183957397938,
	0.183453977108, 0.182949960232, 0.182445406914, 0.181940436363,
	0.181434750557, 0.180928587914, 0.180421829224, 0.179914534092,
	0.179406702518, 0.178898334503, 0.178389430046, 0.177879989147,
	0.177369952202, 0.176859438419, 0.176348328590, 0.175836741924,
	0.175324618816, 0.174811959267, 0.174298763275, 0.173785090446,
	0.173270881176, 0.172756135464, 0.172240853310, 0.171725094318,
	0.171208739281, 0.170691967010, 0.170174598694, 0.169656693935,
	0.169138371944, 0.168619513512, 0.168100118637, 0.167580246925,
	0.167059898376, 0.166539072990, 0.166017711163, 0.165495812893,
	0.164973437786, 0.164450585842, 0.163927257061, 0.163403451443,
	0.162879049778, 0.162354290485, 0.161828994751, 0.161303222179,
	0.160776913166, 0.160250246525, 0.159723043442, 0.159195303917,
	0.158667147160, 0.158138513565, 0.157609403133, 0.157079756260,
	0.156549692154, 0.156019270420, 0.155488193035, 0.154956758022,
	0.154424905777, 0.153892517090, 0.153359651566, 0.152826309204,
	0.152292609215, 0.151758432388, 0.151223719120, 0.150688648224,
	0.150153040886, 0.149617075920, 0.149080574512, 0.148543596268,
	0.148006320000, 0.147468507290, 0.146930217743, 0.146391570568,
	0.145852446556, 0.145312905312, 0.144772827625, 0.144232392311,
	0.143691599369, 0.143150269985, 0.142608463764, 0.142066359520,
	0.141523718834, 0.140980720520, 0.140437245369, 0.139893352985,
	0.139349043369, 0.138804316521, 0.138259172440, 0.137713611126,
	0.137167632580, 0.136621177197, 0.136074364185, 0.135527133942,
	0.134979486465, 0.134431421757, 0.133882939816, 0.133334040642,
	0.132784724236, 0.132234990597, 0.131684958935, 0.131134390831,
	0.130583465099, 0.130032181740, 0.129480421543, 0.128928303719,
	0.128375828266, 0.127822816372, 0.127269506454, 0.126715779305,
	0.126161694527, 0.125607132912, 0.125052213669, 0.124496996403,
	0.123941242695, 0.123385190964, 0.122828722000, 0.122271895409,
	0.121714651585, 0.121156990528, 0.120598971844, 0.120040595531,
	0.119481861591, 0.118922710419, 0.118363082409, 0.117803215981,
	0.117242932320, 0.116682231426, 0.116121232510, 0.115559816360,
	0.114997982979, 0.114435851574, 0.113873243332, 0.113310396671,
	0.112747073174, 0.112183392048, 0.111619412899, 0.111055016518,
	0.110490322113, 0.109925150871, 0.109359681606, 0.108793854713,
	0.108227670193, 0.107661128044, 0.107094168663, 0.106526911259,
	0.105959236622, 0.105391263962, 0.104822933674, 0.104254245758,
	0.103685200214, 0.103115737438, 0.102545976639, 0.101975917816,
	0.101405382156, 0.100834667683, 0.100263476372, 0.099691987038,
	0.099120140076, 0.098547875881, 0.097975373268, 0.097402453423,
	0.096829295158, 0.096255660057, 0.095681726933, 0.095107495785,
	0.094532907009, 0.093957960606, 0.093382716179, 0.092807114124,
	0.092231214046, 0.091654956341, 0.091078281403, 0.090501308441,
	0.089924037457, 0.089346468449, 0.088768541813, 0.088190257549,
	0.087611675262, 0.087032735348, 0.086453497410, 0.085873961449,
	0.085294008255, 0.084713757038, 0.084133267403, 0.083552420139,
	0.082971215248, 0.082389712334, 0.081807851791, 0.081225633621,
	0.080643236637, 0.080060362816, 0.079477250576, 0.078893840313,
	0.078310132027, 0.077726066113, 0.077141702175, 0.076556980610,
	0.075971961021, 0.075386703014, 0.074801087379, 0.074215173721,
	0.073628902435, 0.073042333126, 0.072455525398, 0.071868300438,
	0.071280896664, 0.070693075657, 0.070105016232, 0.069516658783,
	0.068927943707, 0.068338930607, 0.067749619484, 0.067160010338,
	0.066570162773, 0.065979957581, 0.065389454365, 0.064798653126,
	0.064207553864, 0.063616216183, 0.063024461269, 0.062432527542,
	0.061840236187, 0.061247646809, 0.060654759407, 0.060061693192,
	0.059468209743, 0.058874547482, 0.058280467987, 0.057686150074,
	0.057091534138, 0.056496620178, 0.055901467800, 0.055306017399,
	0.054710209370, 0.054114162922, 0.053517878056, 0.052921295166,
	0.052324414253, 0.051727175713, 0.051129758358, 0.050532042980,
	0.049934029579, 0.049335718155, 0.048737108707, 0.048138201237,
	0.047539114952, 0.046939730644, 0.046340048313, 0.045740067959,
	0.045139789581, 0.044539213181, 0.043938457966, 0.043337404728,
	0.042736113071, 0.042134463787, 0.041532635689, 0.040930390358,
	0.040328025818, 0.039725303650, 0.039122343063, 0.038519084454,
	0.037915587425, 0.037311792374, 0.036707758904, 0.036103487015,
	0.035498917103, 0.034894108772, 0.034289002419, 0.033683657646,
	0.033078014851, 0.032472133636, 0.031866014004, 0.031259536743,
	0.030652880669, 0.030045926571, 0.029438734055, 0.028831362724,
	0.028223633766, 0.027615666389, 0.027007460594, 0.026398956776,
	0.025790274143, 0.025181293488, 0.024572074413, 0.023962497711,
	0.023352801800, 0.022742748260, 0.022132515907, 0.021522045135,
	0.020911276340, 0.020300269127, 0.019689023495, 0.019077539444,
	0.018465697765, 0.017853736877, 0.017241477966, 0.016628980637,
	0.016016244888, 0.015403211117, 0.014789998531, 0.014176487923,
	0.013562798500, 0.012948811054, 0.012334585190, 0.011720120907,
	0.011105477810, 0.010490477085, 0.009875297546, 0.009259879589,
	0.008644223213, 0.008028268814, 0.007412135601, 0.006795823574,
	0.006179153919, 0.005562305450, 0.004945218563, 0.004327833652,
	0.003710269928, 0.003092467785, 0.002474486828, 0.001856148243,
	0.001237690449, 0.000618994236, 0.000000000000, -0.000619232655,
	-0.001238644123, -0.001858353615, -0.002478241920, -0.003098428249,
	-0.003718733788, -0.004339277744, -0.004960119724, -0.005581140518,
	-0.006202459335, -0.006823956966, -0.007445693016, -0.008067667484,
	-0.008689820766, -0.009312212467, -0.009934842587, -0.010557651520,
	-0.011180758476, -0.011804044247, -0.012427628040, -0.013051390648,
	-0.013675332069, -0.014299511909, -0.014923930168, -0.015548527241,
	-0.016173422337, -0.016798496246, -0.017423748970, -0.018049240112,
	-0.018674969673, -0.019300937653, -0.019927084446, -0.020553469658,
	-0.021180033684, -0.021806895733, -0.022433876991, -0.023061156273,
	-0.023688614368, -0.024316251278, -0.024944126606, -0.025572240353,
	-0.026200532913, -0.026829063892, -0.027457773685, -0.028086721897,
	-0.028715848923, -0.029345214367, -0.029974758625, -0.030604541302,
	-0.031234562397, -0.031864762306, -0.032495141029, -0.033125758171,
	-0.033756554127, -0.034387588501, -0.035018861294, -0.035650253296,
	-0.036281883717, -0.036913752556, -0.037545800209, -0.038178086281,
	-0.038810491562, -0.039443194866, -0.040076017380, -0.040709137917,
	-0.041342377663, -0.041975855827, -0.042609512806, -0.043243408203,
	-0.043877482414, -0.044511735439, -0.045146226883, -0.045780897141,
	-0.046415746212, -0.047050774097, -0.047686040401, -0.048321545124,
	-0.048957169056, -0.049593031406, -0.050229072571, -0.050865292549,
	-0.051501750946, -0.052138388157, -0.052775204182, -0.053412258625,
	-0.054049432278, -0.054686844349, -0.055324435234, -0.055962264538,
	-0.056600213051, -0.057238399982, -0.057876765728, -0.058515310287,
	-0.059154093266, -0.059792995453, -0.060432136059, -0.061071455479,
	-0.061710953712, -0.062350630760, -0.062990486622, -0.063630580902,
	-0.064270794392, -0.064911246300, -0.065551877022, -0.066192686558,
	-0.066833674908, -0.067474842072, -0.068116247654, -0.068757772446,
	-0.069399535656, -0.070041418076, -0.070683538914, -0.071325838566,
	-0.071968257427, -0.072610914707, -0.073253750801, -0.073896765709,
	-0.074539959431, -0.075183331966, -0.075826883316, -0.076470613480,
	-0.077114522457, -0.077758610249, -0.078402876854, -0.079047322273,
	-0.079691946507, -0.080336749554, -0.080981731415, -0.081626832485,
	-0.082272171974, -0.082917690277, -0.083563387394, -0.084209203720,
	-0.084855258465, -0.085501432419, -0.086147844791, -0.086794376373,
	-0.087441086769, -0.088088035583, -0.088735103607, -0.089382350445,
	-0.090029716492, -0.090677320957, -0.091325104237, -0.091973006725,
	-0.092621147633, -0.093269407749, -0.093917787075, -0.094566464424,
	-0.095215201378, -0.095864176750, -0.096513271332, -0.097162544727,
	-0.097811996937, -0.098461627960, -0.099111437798, -0.099761366844,
	-0.100411474705, -0.101061761379, -0.101712167263, -0.102362811565,
	-0.103013575077, -0.103664517403, -0.104315698147, -0.104966938496,
	-0.105618357658, -0.106269955635, -0.106921732426, -0.107573628426,
	-0.108225762844, -0.108877956867, -0.109530389309, -0.110182940960,
	-0.110835671425, -0.111488580704, -0.112141609192, -0.112794876099,
	-0.113448202610, -0.114101707935, -0.114755392075, -0.115409255028,
	-0.116063296795, -0.116717457771, -0.117371797562, -0.118026256561,
	-0.118680894375, -0.119335711002, -0.119990646839, -0.120645701885,
	-0.121300995350, -0.121956408024, -0.122611939907, -0.123267710209,
	-0.123923599720, -0.124579608440, -0.125235795975, -0.125892102718,
	-0.126548647881, -0.127205252647, -0.127862036228, -0.128518998623,
	-0.129176080227, -0.129833400249, -0.130490779877, -0.131148338318,
	-0.131806015968, -0.132463872433, -0.133121848106, -0.133780062199,
	-0.134438335896, -0.135096788406, -0.135755360126, -0.136414170265,
	-0.137073040009, -0.137732088566, -0.138391256332, -0.139050662518,
	-0.139710128307, -0.140369713306, -0.141029477119, -0.141689419746,
	-0.142349541187, -0.143009781837, -0.143670141697, -0.144330620766,
	-0.144991278648, -0.145652055740, -0.146312952042, -0.146974027157,
	-0.147635281086, -0.148296654224, -0.148958146572, -0.149619817734,
	-0.150281608105, -0.150943517685, -0.151605606079, -0.152267873287,
	-0.152930200100, -0.153592705727, -0.154255390167, -0.154918134212,
	-0.155581057072, -0.156244099140, -0.156907320023, -0.157570660114,
	-0.158234119415, -0.158897757530, -0.159561455250, -0.160225391388,
	-0.160889446735, -0.161553561687, -0.162217855453, -0.162882328033,
	-0.163546860218, -0.164211571217, -0.164876461029, -0.165541410446,
	-0.166206538677, -0.166871786118, -0.167537152767, -0.168202698231,
	-0.168868362904, -0.169534146786, -0.170200049877, -0.170866131783,
	-0.171532273293, -0.172198593616, -0.172865033150, -0.173531651497,
	-0.174198389053, -0.174865186214, -0.175532162189, -0.176199316978,
	-0.176866531372, -0.177533924580, -0.178201436996, -0.178869009018,
	-0.179536819458, -0.180204689503, -0.180872738361, -0.181540846825,
	-0.182209134102, -0.182877540588, -0.183546066284, -0.184214711189,
	-0.184883534908, -0.185552418232, -0.186221480370, -0.186890661716,
	-0.187559902668, -0.188229382038, -0.188898921013, -0.189568579197,
	-0.190238356590, -0.190908312798, -0.191578388214, -0.192248523235,
	-0.192918837070, -0.193589210510, -0.194259762764, -0.194930493832,
	-0.195601284504, -0.196272194386, -0.196943223476, -0.197614312172,
	-0.198285639286, -0.198957026005, -0.199628591537, -0.200300216675,
	-0.200972020626, -0.201643884182, -0.202315866947, -0.202988028526,
	-0.203660309315, -0.204332649708, -0.205005109310, -0.205677747726,
	-0.206350505352, -0.207023322582, -0.207696318626, -0.208369374275,
	-0.209042608738, -0.209715902805, -0.210389316082, -0.211062908173,
	-0.211736559868, -0.212410390377, -0.213084280491, -0.213758289814,
	-0.214432477951, -0.215106725693, -0.215781092644, -0.216455549002,
	-0.217130184174, -0.217804878950, -0.218479722738, -0.219154685736,
	-0.219829738140, -0.220504909754, -0.221180200577, -0.221855610609,
	-0.222531110048, -0.223206758499, -0.223882496357, -0.224558383226,
	-0.225234329700, -0.225910425186, -0.226586610079, -0.227262884378,
	-0.227939307690, -0.228615850210, -0.229292482138, -0.229969263077,
	-0.230646073818, -0.231323093176, -0.232000142336, -0.232677340508,
	-0.233354657888, -0.234032064676, -0.234709590673, -0.235387206078,
	-0.236064910889, -0.236742764711, -0.237420737743, -0.238098770380,
	-0.238776952028, -0.239455252886, -0.240133613348, -0.240812093019,
	-0.241490721703, -0.242169409990, -0.242848247290, -0.243527114391,
	-0.244206160307, -0.244885295630, -0.245564520359, -0.246243864298,
	-0.246923267841, -0.247602820396, -0.248282492161, -0.248962253332,
	-0.249642103910, -0.250322073698, -0.251002162695, -0.251682341099,
	-0.252362608910, -0.253042995930, -0.253723442554, -0.254404038191,
	-0.255084723234, -0.255765527487, -0.256446391344, -0.257127404213,
	-0.257808506489, -0.258489698172, -0.259170979261, -0.259852379560,
	-0.260533869267, -0.261215478182, -0.261897146702, -0.262578964233,
	-0.263260871172, -0.263942837715, -0.264624953270, -0.265307158232,
	-0.265989482403, -0.266671866179, -0.267354339361, -0.268036931753,
	-0.268719583750, -0.269402414560, -0.270085275173, -0.270768254995,
	-0.271451354027, -0.272134482861, -0.272817790508, -0.273501127958,
	-0.274184614420, -0.274868160486, -0.275552004576, -0.276246696711,
	-0.276955842972, -0.277676463127, -0.278406113386, -0.279142975807,
	-0.279885530472, -0.280632615089, -0.281383335590, -0.282136917114,
	-0.282892853022, -0.283650636673, -0.284409940243, -0.285170495510,
	-0.285932064056, -0.286694467068, -0.287457615137, -0.288221299648,
	-0.288985580206, -0.289750248194, -0.290515333414, -0.291280776262,
	-0.292046546936, -0.292812615633, -0.293578982353, -0.294345587492,
	-0.295112460852, -0.295879513025, -0.296646833420, -0.297414392233,
	-0.298182129860, -0.298950135708, -0.299718320370, -0.300486743450,
	-0.301255315542, -0.302024126053, -0.302793115377, -0.303562343121,
	-0.304331749678, -0.305101364851, -0.305871158838, -0.306641131639,
	-0.307411372662, -0.308181732893, -0.308952361345, -0.309723138809,
	-0.310494124889, -0.311265289783, -0.312036663294, -0.312808215618,
	-0.313580006361, -0.314351975918, -0.315124064684, -0.315896451473,
	-0.316668987274, -0.317441672087, -0.318214595318, -0.318987697363,
	-0.319761008024, -0.320534497499, -0.321308195591, -0.322082072496,
	-0.322856098413, -0.323630332947, -0.324404805899, -0.325179398060,
	-0.325954228640, -0.326729238033, -0.327504426241, -0.328279823065,
	-0.329055398703, -0.329831123352, -0.330607086420, -0.331383198500,
	-0.332159548998, -0.332936048508, -0.333712756634, -0.334489613771,
	-0.335266709328, -0.336043953896, -0.336821347475, -0.337598979473,
	-0.338376790285, -0.339154779911, -0.339932948351, -0.340703547001,
	-0.341460168362, -0.342204123735, -0.342936009169, -0.343656539917,
	-0.344366520643, -0.345066785812, -0.345758140087, -0.346441298723,
	-0.347117066383, -0.347785979509, -0.348448574543, -0.349105387926,
	-0.349756926298, -0.350403457880, -0.351045399904, -0.351682960987,
	-0.352316498756, -0.352946251631, -0.353572368622, -0.354195058346,
	-0.354814529419, -0.355430841446, -0.356044232845, -0.356654793024,
	-0.357262581587, -0.357867747545, -0.358470380306, -0.359070599079,
	-0.359668403864, -0.360263943672, -0.360857278109, -0.361448466778,
	-0.362037539482, -0.362624615431, -0.363209664822, -0.363792806864,
	-0.364374041557, -0.364953458309, -0.365531116724, -0.366106957197,
	-0.366681158543, -0.367253661156, -0.367824524641, -0.368393778801,
	-0.368961453438, -0.369527608156, -0.370092272758, -0.370655417442,
	-0.371217161417, -0.371777445078, -0.372336328030, -0.372893840075,
	-0.373450040817, -0.374004870653, -0.374558418989, -0.375110656023,
	-0.375661671162, -0.376211434603, -0.376759976149, -0.377307295799,
	-0.377853453159, -0.378398448229, -0.378942251205, -0.379484951496,
	-0.380026549101, -0.380567044020, -0.381106466055, -0.381644785404,
	-0.382182061672, -0.382718294859, -0.383253514767, -0.383787691593,
	-0.384320884943, -0.384853094816, -0.385384321213, -0.385914593935,
	-0.386443912983, -0.386972278357, -0.387499719858, -0.388026237488,
	-0.388551831245, -0.389076471329, -0.389600366354, -0.390123337507,
	-0.390645444393, -0.391166716814, -0.391687124968, -0.392206668854,
	-0.392725408077, -0.393243283033, -0.393760412931, -0.394276678562,
	-0.394792169333, -0.395306885242, -0.395820796490, -0.396333962679,
	-0.396846383810, -0.397358000278, -0.397868901491, -0.398379057646,
	-0.398888468742, -0.399397134781, -0.399905115366, -0.400412350893,
	-0.400918900967, -0.401424735785, -0.401929885149, -0.402434349060,
	-0.402938157320, -0.403441250324, -0.403943657875, -0.404445439577,
	-0.404946535826, -0.405447006226, -0.405946820974, -0.406445980072,
	-0.406944513321, -0.407442390919, -0.407939672470, -0.408436328173,
	-0.408932358027, -0.409427791834, -0.409922599792, -0.410416811705,
	-0.410910427570, -0.411403477192, -0.411895900965, -0.412387788296,
	-0.412879049778, -0.413369774818, -0.413859903812, -0.414349466562,
	-0.414838492870, -0.415326923132, -0.415814846754, -0.416302174330,
	-0.416788995266, -0.417275249958, -0.417760968208, -0.418246179819,
	-0.418730825186, -0.419214993715, -0.419698596001, -0.420181691647,
	-0.420664280653, -0.421146333218, -0.421627908945, -0.422108948231,
	-0.422589510679, -0.423069566488, -0.423549115658, -0.424028217793,
	-0.424506783485, -0.424984872341, -0.425462514162, -0.425939619541,
	-0.426416277885, -0.426892489195, -0.427368193865, -0.427843451500,
	-0.428318232298, -0.428792566061, -0.429266452789, -0.429739862680,
	-0.430212795734, -0.430685311556, -0.431157380342, -0.431629002094,
	-0.432100176811, -0.432570904493, -0.433041214943, -0.433511078358,
	-0.433980494738, -0.434449523687, -0.434918075800, -0.435386240482,
	-0.435853987932, -0.436321288347, -0.436788171530, -0.437254667282,
	-0.437720745802, -0.438186407089, -0.438651651144, -0.439116477966,
	-0.439580917358, -0.440044969320, -0.440508633852, -0.440971851349,
	-0.441434711218, -0.441897183657, -0.442359238863, -0.442820936441,
	-0.443282216787, -0.443743109703, -0.444203674793, -0.444663792849,
	-0.445123583078, -0.445582956076, -0.446041971445, -0.446500629187,
	-0.446958899498, -0.447416782379, -0.447874337435, -0.448331505060,
	-0.448788315058, -0.449244767427, -0.449700862169, -0.450156599283,
	-0.450611978769, -0.451067000628, -0.451521664858, -0.451975971460,
	-0.452429950237, -0.452883571386, -0.453336834908, -0.453789770603,
	-0.454242378473, -0.454694598913, -0.455146521330, -0.455598086119,
	-0.456049323082, -0.456500232220, -0.456950783730, -0.457401037216,
	-0.457850933075, -0.458300501108, -0.458749771118, -0.459198683500,
	-0.459647268057, -0.460095554590, -0.460543513298, -0.460991144180,
	-0.461438477039, -0.461885482073, -0.462332129478, -0.462778508663,
	-0.463224560022, -0.463670313358, -0.464115768671, -0.464560896158,
	-0.465005695820, -0.465450197458, -0.465894401073, -0.466338336468,
	-0.466781944036, -0.467225223780, -0.467668235302, -0.468110918999,
	-0.468553334475, -0.468995451927, -0.469437271357, -0.469878792763,
	-0.470320016146, -0.470760941505, -0.471201598644, -0.471641957760,
	-0.472082048655, -0.472521811724, -0.472961336374, -0.473400533199,
	-0.473839461803, -0.474278151989, -0.474716484547, -0.475154578686,
	-0.475592404604, -0.476029932499, -0.476467192173, -0.476904183626,
	-0.477340877056, -0.477777302265, -0.478213489056, -0.478649377823,
	-0.479084998369, -0.479520380497, -0.479955434799, -0.480390250683,
	-0.480824828148, -0.481259107590, -0.481693148613, -0.482126891613,
	-0.482560396194, -0.482993602753, -0.483426630497, -0.483859330416,
	-0.484291791916, -0.484723985195, -0.485155940056, -0.485587626696,
	-0.486019074917, -0.486450254917, -0.486881166697, -0.487311840057,
	-0.487742304802, -0.488172471523, -0.488602399826, -0.489032089710,
	-0.489461481571, -0.489890694618, -0.490319639444, -0.490748345852,
	-0.491176784039, -0.491604983807, -0.492032945156, -0.492460697889,
	-0.492888182402, -0.493315428495, -0.493742465973, -0.494169205427,
	-0.494595766068, -0.495022058487, -0.495448112488, -0.495873928070,
	-0.496299535036, -0.496724903584, -0.497150033712, -0.497574925423,
	-0.497999608517, -0.498424023390, -0.498848229647, -0.499272227287,
	-0.499695956707, -0.500119447708, -0.500542759895, -0.500965833664,
	-0.501388669014, -0.501811325550, -0.502233743668, -0.502655923367,
	-0.503077864647, -0.503499567509, -0.503921091557, -0.504342377186,
	-0.504763484001, -0.505184292793, -0.505604982376, -0.506025373936,
	-0.506445586681, -0.506865620613, -0.507285356522, -0.507704913616,
	-0.508124232292, -0.508543372154, -0.508962333202, -0.509381055832,
	-0.509799540043, -0.510217785835, -0.510635912418, -0.511053800583,
	-0.511471390724, -0.511888861656, -0.512306094170, -0.512723147869,
	-0.513139963150, -0.513556599617, -0.513972997665, -0.514389276505,
	-0.514805316925, -0.515221118927, -0.515636682510, -0.516052126884,
	-0.516467332840, -0.516882300377, -0.517297148705, -0.517711758614,
	-0.518126130104, -0.518540382385, -0.518954396248, -0.519368171692,
	-0.519781827927, -0.520195245743, -0.520608425140, -0.521021485329,
	-0.521434307098, -0.521846950054, -0.522259414196, -0.522671699524,
	-0.523083746433, -0.523495614529, -0.523907303810, -0.524318814278,
	-0.524730086327, -0.525141239166, -0.525552153587, -0.525962889194,
	-0.526373445988, -0.526783823967, -0.527194023132, -0.527603983879,
	-0.528013765812, -0.528423428535, -0.528832852840, -0.529242098331,
	-0.529651165009, -0.530060052872, -0.530468702316, -0.530877232552,
	-0.531285583973, -0.531693756580, -0.532101750374, -0.532509565353,
	-0.532917141914, -0.533324599266, -0.533731818199, -0.534138917923,
	-0.534545779228, -0.534952521324, -0.535359084606, -0.535765469074,
	-0.536171674728, -0.536577701569, -0.536983489990, -0.537389159203,
	-0.537794649601, -0.538200020790, -0.538605153561, -0.539010107517,
	-0.539414882660, -0.539819538593, -0.540224015713, -0.540628314018,
	-0.541032433510, -0.541436374187, -0.541840136051, -0.542243719101,
	-0.542647182941, -0.543050467968, -0.543453574181, -0.543856501579,
	-0.544259250164, -0.544661879539, -0.545064270496, -0.545466542244,
	-0.545868635178, -0.546270608902, -0.546672344208, -0.547073960304,
	-0.547475457191, -0.547876715660, -0.548277854919, -0.548678755760,
	-0.549079537392, -0.549480199814, -0.549880683422, -0.550280988216,
	-0.550681114197, -0.551081120968, -0.551480948925, -0.551880598068,
	-0.552280128002, -0.552679419518, -0.553078651428, -0.553477704525,
	-0.553876519203, -0.554275274277, -0.554673850536, -0.555072247982,
	-0.555470466614, -0.555868566036, -0.556266546249, -0.556664288044,
	-0.557061910629, -0.557459414005, -0.557856678963, -0.558253884315,
	-0.558650851250, -0.559047758579, -0.559444427490, -0.559840977192,
	-0.560237407684, -0.560633659363, -0.561029732227, -0.561425685883,
	-0.561821460724, -0.562217116356, -0.562612652779, -0.563007950783,
	-0.563403189182, -0.563798189163, -0.564193129539, -0.564587831497,
	-0.564982473850, -0.565376937389, -0.565771222115, -0.566165387630,
	-0.566559433937, -0.566953301430, -0.567346990108, -0.567740559578,
	-0.568134009838, -0.568527281284, -0.568920433521, -0.569313466549,
	-0.569706320763, -0.570098996162, -0.570491552353, -0.570883989334,
	-0.571276307106, -0.571668446064, -0.572060406208, -0.572452306747,
	-0.572843968868, -0.573235571384, -0.573626995087, -0.574018299580,
	-0.574409425259, -0.574800431728, -0.575191318989, -0.575582027435,
	-0.575972676277, -0.576363086700, -0.576753377914, -0.577143609524,
	-0.577533602715, -0.577923536301, -0.578313291073, -0.578702926636,
	-0.579092383385, -0.579481780529, -0.579870998859, -0.580260038376,
	-0.580649018288, -0.581037819386, -0.581426501274, -0.581815004349,
	-0.582203447819, -0.582591712475, -0.582979857922, -0.583367824554,
	-0.583755731583, -0.584143459797, -0.584531068802, -0.584918558598,
	-0.585305869579, -0.585693120956, -0.586080193520, -0.586467146873,
	-0.586853921413, -0.587240576744, -0.587627172470, -0.588013589382,
	-0.588399887085, -0.588786005974, -0.589172065258, -0.589557945728,
	-0.589943766594, -0.590329408646, -0.590714871883, -0.591100275517,
	-0.591485500336, -0.591870665550, -0.592255651951, -0.592640519142,
	-0.593025267124, -0.593409895897, -0.593794405460, -0.594178795815,
	-0.594563007355, -0.594947099686, -0.595331132412, -0.595714986324,
	-0.596098721027, -0.596482336521, -0.596865832806, -0.597249209881,
	-0.597632408142, -0.598015546799, -0.598398566246, -0.598781406879,
	-0.599164128304, -0.599546790123, -0.599929273129, -0.600311636925,
	-0.600693881512, -0.601076066494, -0.601458072662, -0.601839959621,
	-0.602221727371, -0.602603375912, -0.602984905243, -0.603366315365,
	-0.603747546673, -0.604128718376, -0.604509770870, -0.604890704155,
	-0.605271518230, -0.605652213097, -0.606032729149, -0.606413185596,
	-0.606793522835, -0.607173740864, -0.607553839684, -0.607933819294,
	-0.608313679695, -0.608693420887, -0.609073042870, -0.609452545643,
};

} /* namespace r12AX7 */

#endif /* _DSP_R_12_AX_7_H_ */
