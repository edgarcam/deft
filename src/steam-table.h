// -*- mode: C++; -*-

#pragma once

/*

  Data from:

  E.W. Lemmon, M.O. McLinden and D.G. Friend, "Thermophysical
  Properties of Fluid Systems" in NIST Chemistry WebBook, NIST
  Standard Reference Database Number 69, Eds. P.J. Linstrom and
  W.G. Mallard, National Institute of Standards and Technology,
  Gaithersburg MD, 20899, http://webbook.nist.gov, (retrieved December
  15, 2010).

*/

static double temperatures_kelvin[] = {
  274,
  275,
  276,
  277,
  278,
  279,
  280,
  281,
  282,
  283,
  284,
  285,
  286,
  287,
  288,
  289,
  290,
  291,
  292,
  293,
  294,
  295,
  296,
  297,
  298,
  299,
  300,
  310,
  320,
  330,
  340,
  350,
  360,
  370,
  380,
  390,
  400,
  410,
  420,
  430,
  440,
  450,
  460,
  470,
  480,
  490,
  500,
  510,
  520,
  530,
  540,
  550,
  560,
  570,
  580,
  590,
  600,
  610,
  613,
  616,
  619,
  622,
  625,
  628,
  631,
  634,
  637,
  640,
  643,
  646,
  647.096,
  0 // the zero temperature marks the end of the data!
};

static double water_saturation_liquid[] = {
  0.0049527405,
  0.0049529592,
  0.0049530932,
  0.0049531452,
  0.0049531178,
  0.0049530135,
  0.0049528344,
  0.0049525829,
  0.004952261,
  0.0049518705,
  0.0049514134,
  0.0049508915,
  0.0049503063,
  0.0049496595,
  0.0049489525,
  0.0049481869,
  0.0049473639,
  0.004946485,
  0.0049455513,
  0.0049445641,
  0.0049435244,
  0.0049424335,
  0.0049412922,
  0.0049401017,
  0.004938863,
  0.0049375768,
  0.0049362442,
  0.0049205351,
  0.0049009462,
  0.004877976,
  0.004851987,
  0.0048232486,
  0.0047919635,
  0.0047582842,
  0.0047223235,
  0.0046841621,
  0.004643853,
  0.0046014248,
  0.0045568835,
  0.0045102136,
  0.0044613787,
  0.0044103205,
  0.0043569581,
  0.0043011862,
  0.004242872,
  0.0041818518,
  0.0041179253,
  0.0040508483,
  0.0039803224,
  0.003905981,
  0.0038273678,
  0.0037439074,
  0.0036548596,
  0.0035592504,
  0.0034557607,
  0.0033425412,
  0.0032168704,
  0.0030744097,
  0.003027367,
  0.0029778219,
  0.0029253456,
  0.0028693641,
  0.0028090919,
  0.0027434354,
  0.0026708613,
  0.0025892341,
  0.0024955994,
  0.0023852479,
  0.0022446572,
  0.0019960588,
  0.00159503241883728
};

static double water_vapor_density[] = {
  2.54776141155443E-008,
  2.72772802640033E-008,
  2.91873564247833E-008,
  3.12135191778055E-008,
  3.33616743423793E-008,
  3.56379632203582E-008,
  0.000000038,
  4.0600722560858E-008,
  4.33007099437285E-008,
  4.61558777030002E-008,
  4.91736399173657E-008,
  5.23616845877339E-008,
  5.5727980174249E-008,
  5.92807821618671E-008,
  0.000000063,
  0.000000067,
  7.11452252735057E-008,
  7.55325792523699E-008,
  8.0152253711186E-008,
  0.000000085,
  9.01293639747485E-008,
  9.55080400411567E-008,
  1.01161530422791E-007,
  1.07101325500064E-007,
  1.13339275705723E-007,
  1.19887598144594E-007,
  1.26758883186558E-007,
  2.16284537284245E-007,
  0.000000355,
  5.6257918697069E-007,
  8.63905080233278E-007,
  1.28934432782099E-006,
  1.87530353730918E-006,
  2.66458396309341E-006,
  3.70672074463512E-006,
  5.05831336654079E-006,
  6.78338351115732E-006,
  8.95380086929423E-006,
  1.16498221648145E-005,
  1.49607953756663E-005,
  1.89860915284804E-005,
  2.38363415600388E-005,
  2.96350779478181E-005,
  3.65209117473763E-005,
  4.46504217832206E-005,
  0.000054202,
  0.000065381,
  7.84267374029446E-005,
  0.000093622,
  0.0001113063,
  0.0001318952,
  0.0001559076,
  0.0001840079,
  0.0002170712,
  0.0002562904,
  0.0003033609,
  0.0003608257,
  0.000432785,
  0.0004581583,
  0.0004857395,
  0.0005158935,
  0.0005490903,
  0.0005859529,
  0.0006273367,
  0.0006744725,
  0.0007292496,
  0.0007948539,
  0.000877493,
  0.0009925767,
  0.0012059924,
  0.00159503241883728
};

static double water_saturated_surface_tension[] = {
  4.85124604182821E-005,
  4.84222095271311E-005,
  4.83315782235334E-005,
  4.82405672421574E-005,
  4.81491773201124E-005,
  4.80574091969483E-005,
  4.79652636121508E-005,
  4.78727413177949E-005,
  4.77798430532377E-005,
  4.76865695705542E-005,
  4.75929216216267E-005,
  4.74988999611634E-005,
  4.74045053461849E-005,
  4.73097385363455E-005,
  4.72146002939326E-005,
  4.71190913838673E-005,
  4.70232125742179E-005,
  0.000046927,
  4.6830348335762E-005,
  4.67333644565163E-005,
  0.000046636,
  4.65382970684307E-005,
  4.64402151241172E-005,
  4.63417687272228E-005,
  0.000046243,
  4.6143785735446E-005,
  4.60442507291124E-005,
  4.50291633396338E-005,
  4.39787642575761E-005,
  4.28938980240654E-005,
  4.17754422203957E-005,
  4.06243101207491E-005,
  3.94414536018367E-005,
  3.82278664379236E-005,
  3.69845880261976E-005,
  3.57127075874437E-005,
  3.44133689447935E-005,
  3.30877760090091E-005,
  3.17371990409574E-005,
  0.000030363,
  2.89665507633896E-005,
  2.75494235101338E-005,
  2.61132219763859E-005,
  2.46596856524927E-005,
  2.31906885146178E-005,
  2.17082593793862E-005,
  2.02146069666249E-005,
  1.87121510447311E-005,
  1.72035617525833E-005,
  1.56918101232508E-005,
  1.41802341386382E-005,
  1.26726271813038E-005,
  1.11733594301009E-005,
  9.68754999148971E-006,
  8.22132028242856E-006,
  6.78218536110614E-006,
  5.37969657427005E-006,
  4.02658737143302E-006,
  3.63279609783118E-006,
  3.2456501071719E-006,
  2.86587816374686E-006,
  2.49433666816908E-006,
  2.13205063207587E-006,
  1.78027524981818E-006,
  1.44059335158634E-006,
  1.11508044251213E-006,
  8.06611215717945E-007,
  5.19510947846267E-007,
  2.61282872845247E-007,
  0.00000005,
  0
};

static double water_vapor_pressure[] = {
  2.20936E-011,
  2.37399E-011,
  2.54939E-011,
  2.73616E-011,
  2.93493E-011,
  3.14636E-011,
  3.37113E-011,
  3.60995E-011,
  3.86358E-011,
  4.13279E-011,
  4.41839E-011,
  4.72122E-011,
  5.04217E-011,
  5.38214E-011,
  5.74210E-011,
  6.12302E-011,
  6.52595E-011,
  6.95194E-011,
  7.40210E-011,
  7.87760E-011,
  8.37962E-011,
  8.90940E-011,
  9.46823E-011,
  1.00574E-010,
  1.06784E-010,
  1.13326E-010,
  1.20214E-010,
  2.11791E-010,
  3.58451E-010,
  5.85063E-010,
  9.24095E-010,
  1.41673E-009,
  2.11392E-009,
  3.07723E-009,
  4.37963E-009,
  6.10600E-009,
  8.35353E-009,
  1.12319E-008,
  1.48634E-008,
  1.93827E-008,
  2.49369E-008,
  3.16850E-008,
  3.97977E-008,
  4.94572E-008,
  6.08569E-008,
  7.42010E-008,
  8.97045E-008,
  1.07593E-007,
  1.28104E-007,
  1.51486E-007,
  1.77999E-007,
  2.07919E-007,
  2.41537E-007,
  2.79162E-007,
  3.21130E-007,
  3.67805E-007,
  4.19592E-007,
  4.76957E-007,
  4.95333E-007,
  5.14277E-007,
  5.33807E-007,
  5.53941E-007,
  5.74701E-007,
  5.96109E-007,
  6.18190E-007,
  6.40973E-007,
  6.64493E-007,
  6.88801E-007,
  7.13973E-007,
  7.40114E-007,
  7.49931530650755e-07
};
