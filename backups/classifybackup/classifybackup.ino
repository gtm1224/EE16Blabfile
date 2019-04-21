/*
 * classify.ino
 *
 * EE16B Spring 2016
 * Emily Naviasky & Nathaniel Mailoa
 *
 * EE 16B Fall 2017
 * Andrew Blatner
 *
 */

#define MIC_INPUT                   P6_0

#define SIZE                        2752
#define SIZE_AFTER_FILTER           172
#define ADC_TIMER_MS                0.35

/*---------------------------*/
/*      CODE BLOCK PCA1      */
/*---------------------------*/

// Enveloping and K-means constants
#define SNIPPET_SIZE                105
#define PRELENGTH                   35
#define THRESHOLD                   0.2

#define KMEANS_THRESHOLD            0.04
#define LOUDNESS_THRESHOLD          300

/*---------------------------*/
/*      CODE BLOCK PCA2      */
/*---------------------------*/

float pca_vec1[SNIPPET_SIZE] = {0.0104505767806, 0.0106322943158, 0.00940495952846, 0.0138419486171, 0.00592448998216, 0.00894845743088, 0.0114578607685, 0.0112163356164, 0.00655275704458, 0.00785499429167, 0.00731257730938, 0.0111046747799, 0.0107046307764, 0.00747645773588, 0.0125904880039, 0.0128759708098, 0.0113814042002, 0.0138793819314, 0.00640536377438, 0.00831209740205, 0.00876540700344, 0.00436334039034, 0.00664558076624, 0.00739849238235, 0.0053281919233, 0.0044308020625, 0.00232645752722, 0.00213771146351, 0.00233148847078, 0.0034125001129, 0.0006328297995, 0.00246935413597, 0.00172176505513, 0.00240296382908, -0.00114163616943, 0.395047564663, 0.553017455257, 0.374829495519, 0.355314867515, 0.201599118525, 0.188156239094, 0.119027180586, 0.0818872397212, 0.0340556801867, 0.00345944652078, -0.0120894711781, -0.013839424144, -0.0346941350588, -0.0625202262781, -0.0758683632374, -0.0727748765878, -0.0866107852169, -0.0793229868362, -0.0916445786303, -0.0847984452865, -0.0835688619573, -0.0819703218438, -0.0792238941697, -0.0725904513518, -0.0712296262543, -0.0599597269062, -0.0567059937006, -0.0508132665298, -0.0490620569515, -0.0366864917491, -0.033053068374, -0.0287517206157, -0.0222101613698, -0.0221568661377, -0.0256757596999, -0.022841353234, -0.0181676992963, -0.0186320688603, -0.0241221383837, -0.0262605915401, -0.00769294926729, 0.0105296075444, 0.0434640171461, 0.0522075065454, 0.0363033115921, 0.0260569748525, -0.00945910626798, -0.0210151976979, -0.0407510768023, -0.0616768589064, -0.0580738947738, -0.0573765699854, -0.0587669390773, -0.0623582270631, -0.0636528723972, -0.064023298984, -0.0780531255971, -0.0744210119058, -0.0708948214662, -0.063606742407, -0.0560533298574, -0.0595049051908, -0.0509875582179, -0.0472565085851, -0.041925360937, -0.0414942189351, -0.0349888653771, -0.0375957064319, -0.0339078972452, -0.033126220365};
float pca_vec2[SNIPPET_SIZE] = {0.00391159005826, 0.000943781069015, 0.000469086036102, 0.00734610244378, -0.000320982723065, 0.00108000180416, 0.00600133586977, 0.00302297166657, -0.00609994297344, -0.00655652226559, -0.00662927217139, -0.00311495851269, -0.00452649627713, -0.00615095079221, -0.00437781470997, -0.00179970706084, -0.00267357027339, 0.000120411951801, -0.000561154335035, -0.0032233416748, 0.000349867655953, -0.0010469700297, 0.00415402891454, 0.00375154578103, 0.00413548759671, 0.00149110483688, 0.00509619444021, 0.0014842448551, -0.000131704237766, -0.000439959423838, -0.00433443716369, -0.00261665596786, -0.00460921004452, -0.00373767409489, 0.000641703354058, -0.0221902651785, -0.00129955633351, 0.14224220908, 0.111638704469, 0.111635524728, 0.130679757596, 0.115420990074, 0.131975180039, 0.0841578561794, 0.0827293236975, 0.0871897265826, 0.0944751470201, 0.100347072606, 0.11098470426, 0.105275433489, 0.0930967034866, 0.0974587849955, 0.0792871908301, 0.0705561445261, 0.0726733488848, 0.075356505955, 0.0717669393991, 0.0711624520169, 0.0691018502349, 0.0684848907922, 0.0664619842727, 0.0549271558791, 0.0480201195146, 0.0243199503997, 0.0161283758721, -0.00407008653188, -0.00161879983032, -0.0108218957225, -0.0115376095175, -0.0173689461021, -0.0280602056836, -0.0329296352892, -0.0559065253504, -0.0458617649719, -0.0539580772867, -0.193185647931, -0.306255660321, -0.438572697892, -0.395245268804, -0.349414112126, -0.30011960122, -0.182351112449, -0.139754278963, -0.0669711475127, -0.0319358200313, -0.0207042947109, -0.0166894249477, -0.0138092650549, -0.00329547912058, 0.0108109384125, 0.0138509179925, 0.0344001650752, 0.0252350212667, 0.0405540317844, 0.0322494517181, 0.0208678816283, 0.0336361157289, 0.0222627049305, 0.0200306781627, 0.0171726145667, 0.0185382679736, 0.0210176078027, 0.0217903147915, 0.027821013512, 0.015087293054};
float mean_vec[SNIPPET_SIZE] = {0.00340342102959, 0.00365571768404, 0.00366588843635, 0.00365405871409, 0.00332396449892, 0.00318787450112, 0.00324373196505, 0.00334188482345, 0.00305887316665, 0.00302196991792, 0.0033668383773, 0.00338855663088, 0.00348836524658, 0.0033301616772, 0.00363096107867, 0.00346422489767, 0.00372970145088, 0.00379592266641, 0.00357015591806, 0.00356491186801, 0.00354576999436, 0.00350274987668, 0.00345498661297, 0.00328180216886, 0.00332785243703, 0.00341110851164, 0.0038627459362, 0.0033909286671, 0.00375918599121, 0.00371534573805, 0.00355694793216, 0.00349118253221, 0.00369898251997, 0.00378872871031, 0.00538919404158, 0.0319547893053, 0.0443282167565, 0.0306732453554, 0.0304929232903, 0.0204742279694, 0.0202868204549, 0.0175546881144, 0.0170405640751, 0.0143582353272, 0.0128907431892, 0.0124356424856, 0.0126281613125, 0.013693244511, 0.0132087078789, 0.0131794383705, 0.0128059897519, 0.0131832106273, 0.0126501870967, 0.0129128833358, 0.0126132296126, 0.0121947716249, 0.0120812824892, 0.0115635183943, 0.0115291455245, 0.01106506912, 0.00994170538274, 0.0095152357496, 0.0092729733481, 0.00803412849965, 0.00725330865876, 0.00647886370029, 0.00613265204031, 0.00596826872155, 0.00594454087584, 0.00610185937164, 0.0060844942585, 0.00637308981507, 0.00770502160216, 0.00729966045255, 0.00761521879093, 0.0106199634408, 0.0129258386301, 0.0165658729472, 0.0170116065751, 0.0163731404631, 0.0150456978347, 0.0134693901725, 0.0128848336369, 0.0134422406471, 0.0139251628866, 0.0129920308935, 0.0124591473793, 0.0116577895476, 0.0111280796856, 0.0109702372955, 0.0109342814349, 0.0115540889132, 0.0110424642615, 0.0110214627226, 0.00992201338699, 0.00950676238572, 0.00950153038862, 0.0088174873081, 0.00868288634701, 0.00815766537346, 0.00804049896796, 0.00743280987042, 0.00754909101267, 0.00752462270501, 0.00725664942575};
float centroid1[2] = {0.04428735, -0.06436808};
float centroid2[2] = {0.12041691, 0.04005789};
float centroid3[2] = {-0.06408656, -0.00971458};
float centroid4[2] = {-0.05649208, 0.04033569};
float* centroids[4] = {
  (float *) &centroid1, (float *) &centroid2,
  (float *) &centroid3, (float *) &centroid4
};

/*---------------------------*/
/*---------------------------*/
/*---------------------------*/

float result[SNIPPET_SIZE] = {0};
float proj1 = 0;
float proj2 = 0;

// Data array and index pointer
int re[SIZE] = {0};
volatile int re_pointer = 0;

/*---------------------------*/
/*       Norm functions      */
/*---------------------------*/

// Compute the L2 norm of (dim1, dim2) and centroid
// input: dim1: 1st dimension coordinate
//        dim2: 2nd dimension coordinate
//        centroid: size-2 array containing centroid coordinates
// output: L2 norm (Euclidean distance) between point and centroid
float l2_norm(float dim1, float dim2, float* centroid) {
  return sqrt(pow(dim1-centroid[0],2) + pow(dim2-centroid[1],2));
}

// Compute the L2 norm of (dim1, dim2, dim3) and centroid
// input: dim1: 1st dimension coordinate
//        dim2: 2nd dimension coordinate
//        dim3: 3rd dimension coordinate
//        centroid: size-3 array containing centroid coordinates
// output: L2 norm (Euclidean distance) between point and centroid
float l2_norm3(float dim1, float dim2, float dim3, float* centroid) {
  return sqrt(pow(dim1-centroid[0],2) + pow(dim2-centroid[1],2) + pow(dim3-centroid[2],2));
}

void setup(void) {
  Serial.begin(38400);

  pinMode(MIC_INPUT, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  re_pointer = 0;
  reset_blinker();
  setTimer();
}

void loop(void) {
  if (re_pointer == SIZE) {
    digitalWrite(RED_LED, LOW);

    // Apply enveloping function and get snippet with speech.
    // Do classification only if loud enough.
    if (envelope(re, result)) {

      // Reset projection result variables declared above
      proj1 = 0;
      proj2 = 0;

      /*---------------------------*/
      /*      CODE BLOCK PCA3      */
      /*---------------------------*/

      // Perform principal component projection
      // YOUR CODE HERE
      for (int i=0; i < SNIPPET_SIZE; i++) {
        result[i] -= mean_vec[i];  
      }
      
      for (int i=0; i<SNIPPET_SIZE; i++){
        proj1 += pca_vec1[i]*result[i];
        proj2 += pca_vec2[i]*result[i];
      }


      // Classification - Hint: use the function l2_norm defined above
      // YOUR CODE HERE
      float dist1 = l2_norm(proj1,proj2,centroid1);
      float dist2 = l2_norm(proj1,proj2,centroid2);
      float dist3 = l2_norm(proj1,proj2,centroid3);
      float dist4 = l2_norm(proj1,proj2,centroid4);

      // Check against KMEANS_THRESHOLD and print result over serial
      // YOUR CODE HERE
      if (dist1 < dist2 && dist1 < dist3 && dist1 < dist4 && dist1 < KMEANS_THRESHOLD) {
        Serial.println("word 1");
      } 
      else if (dist2 < dist1 && dist2 < dist3 && dist2 < dist4 && dist2 < KMEANS_THRESHOLD) {
        Serial.println("word 2");
      }
      else if (dist3 < dist1 && dist3 < dist2 && dist3 < dist4 && dist3 < KMEANS_THRESHOLD) {
        Serial.println("word 3");
      }
      else if (dist4 < dist1 && dist4 < dist2 && dist4 < dist3 && dist4 < KMEANS_THRESHOLD) {
        Serial.println("word 4");
      }

      /*---------------------------*/
      /*---------------------------*/
      /*---------------------------*/
    }

    delay(2000);
    re_pointer = 0;
  }
}

// Enveloping function with thresholding and normalizing,
// returns snippet of interest (containing speech)
bool envelope(int* data, float* data_out) {
  int32_t avg = 0;
  float maximum = 0;
  int32_t total = 0;
  int block;

  // Apply enveloping filter while finding maximum value
  for (block = 0; block < SIZE_AFTER_FILTER; block++) {
    avg = 0;
    for (int i = 0; i < 16; i++) {
      avg += data[i+block*16];
    }
    avg = avg >> 4;
    data[block] = abs(data[block*16] - avg);
    for (int i = 1; i < 16; i++) {
      data[block] += abs(data[i+block*16] - avg);
    }
    if (data[block] > maximum) {
      maximum = data[block];
    }
  }

  // If not loud enough, return false
  if (maximum < LOUDNESS_THRESHOLD) {
    return false;
  }

  // Determine threshold
  float thres = THRESHOLD * maximum;

  // Figure out when interesting snippet starts and write to data_out
  block = PRELENGTH;
  while (data[block++] < thres);
  if (block > SIZE_AFTER_FILTER - SNIPPET_SIZE) {
    block = SIZE_AFTER_FILTER - SNIPPET_SIZE;
  }
  for (int i = 0; i < SNIPPET_SIZE; i++) {
    data_out[i] = data[block-PRELENGTH+i];
    total += data_out[i];
  }

  // Normalize data_out
  for (int i = 0; i < SNIPPET_SIZE; i++) {
    data_out[i] = data_out[i] / total;
  }

  return true;
}

/*---------------------------*/
/*     Helper functions      */
/*---------------------------*/

void reset_blinker(void) {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  delay(100);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  delay(100);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);
  digitalWrite(GREEN_LED, LOW);
}

/*---------------------------*/
/*    Interrupt functions    */
/*---------------------------*/

// ISR for timestep
#pragma vector=TIMER2_A0_VECTOR    // Timer A ISR
__interrupt void Timer2_A0_ISR(void) {
  if (re_pointer < SIZE) {
    digitalWrite(RED_LED, HIGH);
    re[re_pointer] = (analogRead(MIC_INPUT) >> 4) - 128;
    re_pointer += 1;
  }
}

// Set timer for timestep; use A2 since A0 & A1 are used by PWM
void setTimer(void) {
  // Set the timer based on 25MHz clock
  TA2CCR0 = (unsigned int) (25000*ADC_TIMER_MS);
  TA2CCTL0 = CCIE;
  __bis_SR_register(GIE);
  TA2CTL = TASSEL_2 + MC_1 + TACLR + ID_0;
}
