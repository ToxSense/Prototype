# Learning Atmospheres - ToxSense 

## ESP32 - CAM (Headband)

The main headband processor consists of an ESP32-CAM-module. The combination of a camera (OV2640) , a combined wifi/bluetooth chip and enough general purpose input output (GPIO) pins to connect the amplifiers with the body bass exciters made us opt for the microcontroller . Its programming language can be c, c++ or Assembler, but with the often used Arduino IDE it is mostly common to use predefined functions already implemented for usage and correlating libraries. The rough program concept is split in a setup and a loop part. In the setup function all used parts are initiated, so that they are ready to use in the following loop part. There the script reruns in a defined interval, repeating its tasks continuously. 

For usage, the finished, programmed script is flashed into the board's memory via an USB-to-serial adapter.

To use the built in camera module different pre definitions are set, like the picture format in CIF, which stands for a resolution of 352 x 288 Pixel. This format is used to limit the computing resources to a minimum, whilst maintaining a quality high enough for inference actions made by the AIs. For the bluetooth connection between the ESP32-CAM module and the smartphone a serialport is established, with an intern arduino library. This form of connections allows it to stream simple data strings in an efficient way. To receive such strings, a callback listener is implemented, which detects an incoming stream right away.

The magnetic sensor, which provides the users orientation, is connected to two GPIO pins of the microprocessor. It provides its data through an I2C protocol connection, which also has to be set up in the beginning. For the initialisation of the magnetic sensor a library is used to set its register. As the last setup step, the ledcWrite channels, whose define the PWM signals for the body bass exciter are defined, with a custom frequency and bit resolution for the duty cycles. 

In the following loop part of the script, the bluetooth listener detects commands sent by the app and executes special functions connected to them. The most important triggers the camera to take a picture . This picture is encoded to a base64 string, enabling it to be sent over the serialized bluetooth connection to the smartphone. Another command provides the direction based aqi values and updates the PWM channels according to the users heading.

All used hardware parts take in a power supply about five voltages, which can be easily provided over USB, connected to a power bank.

## Credits

This project is imagined and created by Timo Bilhöfer, Markus Pfaff and Maria Rădulescu.

As part of the seminar *Learning Atmospheres* in the winter-semester 2020/21 it is supported by Irina Auernhammer, Silas Kalmbach and Prof. Lucio Blandini from the[ Institute for Lightweight Structures and Conceptual Design (**ILEK**)](https://www.ilek.uni-stuttgart.de/) and is also part of the[ Collaborative Research Centre 1244 (**SFB 1244**)](https://www.sfb1244.uni-stuttgart.de/).



## **Bibliography**

Air Protection and **Climate Policy Office**, Department of Air Quality and Education Monitoring in **Warsaw** (**2020**), Warsaw’s holistic approach to reduce air pollution, https://breathelife2030.org/news/warsaws-holistic-approach-reduce-air-pollution/ Accessed 2021/03/16



**Badach**, Joanna; Voordeckers, Dimitri; Nyka, Lucyna; van Acker, Maarten (**2020**): A framework for Air Quality Management Zones - Useful GIS-based tool for urban planning: Case studies in Antwerp and Gdańsk. In Building and Environment 174, p. 106743. DOI: 10.1016/j.buildenv.2020.106743.



**BreathLife** (**2016**): https://breathelife2030.org/solutions/citywide-solutions/ Accessed 2021/03/16



**Climate & Clean Air Coalition** (**2020**): World Cities day event focuses on how health, climate and urban air pollution are interlinked, https://breathelife2030.org/news/world-cities-day-event-focuses-health-climate-urban-air-pollution-interlinked/ Accessed 2021/03/16



**Das**, Ritwajit (**2020**) How community-based air quality monitoring is helping the city of Bengaluru fight back against air pollution, https://breathelife2030.org/news/community-based-air-quality-monitoring-helping-city-bengaluru-fight-back-air-pollution/ Accessed 2021/03/16

**Goodfellow**, Ian; Bengio, Yoshua; Courville Aaron. (**2016**): *Deep Learning*. MIT Press.

**Institute of hygiene and environment, Hamburg**. Leuchtbakterientest. Accessed 2021/03/17. https://www.hamburg.de/hu/biotestverfahren/2604448/leuchtbakterientest/

**Kang**, Gaganjot Kaur; Gao, Jerry Zeyu; Chiao, Sen; Lu, Shengqiang; Xie, Gang (**2018**): Air Quality Prediction: Big Data and Machine Learning Approaches. In IJESD 9 (1), pp. 8–16. DOI: 10.18178/ijesd.2018.9.1.1066.

**Larson**, Jeff; Mattu, Surya; Kirchner, Lauren; Angwin, Julia (**2016**): How We Analyzed the COMPAS Recidivism Algorithm

**Liao**, Xiong; Tu, Hong; Maddock, Jay E.; Fan, Si; Lan, Guilin; Wu, Yanyan et al. (**2015**): Residents’ perception of air quality, pollution sources, and air pollution control in Nanchang, China. In Atmospheric Pollution Research 6 (5), pp. 835–841. DOI: 10.5094/APR.2015.092.

**Nikolopoulou**, Marialena. (**2009**). PERCEPTION OF AIR POLLUTION AND COMFORT IN THE URBAN, Conference: CISBAT International Scientific Conference, Lausanne

**Nisky**, Ilana; Hartcher-O’Brien, Jess; Wiertlewski, Michaël; Smeets, Jeroen (**2020**): Haptics: Science, Technology, Applications. Cham: Springer International Publishing (12272).

**Peng**, Minggang; Zhang, Hui; Evans, Richard D.; Zhong, Xiaohui; Yang, Kun (**2019**): Actual Air Pollution, Environmental Transparency, and the Perception of Air Pollution in China. In *The Journal of Environment & Development* 28 (1), pp. 78–105. DOI: 10.1177/1070496518821713.

**Rosenblatt**, Frank (**1958**): The perceptron - a probabilistic model for information storage and organization in the brain.

**Smedley**, Tim. **2019**/11/15. The toxic killers in our air too small to see. Accessed 2021/03/14. https://www.bbc.com/future/article/20191113-the-toxic-killers-in-our-air-too-small-to-see

**Sokhanvar**, Saeed S. (**2013**): Tactile sensing and displays. Haptic feedback for minimally invasive surgery and robotics. Chichester, West Sussex, U.K.: John Wiley & Sons.

**Spiroska**, Jana; Rahman, Asif; Pal, Saptarshi (**2011**): Air Pollution in Kolkata: An Analysis of Current Status and Interrelation between Different Factors. In South East European University Review 8 (1). DOI: 10.2478/v10306-012-0012-7.

**Valueva**, M.V.; Nagornov N.N.; Lyakhov, P.A.; Valuev, G.V.; Chervyakov, N.I. (**2020**) Application of the residue number system to reduce hardware costs of the convolutional neural network implementation, Mathematics and Computers in Simulation. https://doi.org/10.1016/j.matcom.2020.04.031.

**VISATON**. **2010**/04. Basic principles of exciter-technology. Accessed 2021/03/14. 

**Vu**, Tuan V.; Shi, Zongbo; Cheng, Jing; Zhang, Qiang; He, Kebin; Wang, Shuxiao; Harrison, Roy M. (**2019**): Assessing the impact of clean air action on air quality trends in Beijing using a machine learning technique. In Atmos. Chem. Phys. 19 (17), pp. 11303–11314. DOI: 10.5194/acp-19-11303-2019.

**World Health Organization (2006)**: Air quality guidelines. Global update 2005 : particulate matter, ozone, nitrogen dioxide, and sulfur dioxide. Copenhagen: World Health Organization.

**Whitney**, Matt; Quin, Hu (**2021**): How China is tackling air pollution with big data, https://breathelife2030.org/news/china-tackling-air-pollution-big-data/ Accessed 2021/03/16

**Wu**, Yi-Chen; Shiledar, Ashutosh; Li, Yi-Cheng; Wong, Jeffrey; Feng, Steve; Chen, Xuan et al. (**2017**): Air quality monitoring using mobile microscopy and machine learning. In Light, science & applications 6 (9), e17046. DOI: 10.1038/lsa.2017.46.



## **Programming resources:**

**1** **Android Bluetooth**. Majdi_la. (Sample Code) https://stackoverflow.com/questions/13450406/how-to-receive-serial-data-using-android-bluetooth. CC BY-SA 3.0.

**2** **Android GPS**. Azhar. (Sample Code) https://www.tutorialspoint.com/how-to-get-the-current-gps-location-programmatically-on-android-using-kotlin. Terms apply.

**3** **Android TFlite**. Anupamchugh. (Sample Code) anupamchugh/AndroidTfLiteCameraX. Pending request.

**4** **FastAPI**. Sebastián Ramírez. (Library) tiangolo/fastapi. https://fastapi.tiangolo.com/. MIT-License.

**5** **I2cdevlib**. Jeff Rowberg. (Library) jrowberg/i2cdevlib. MIT-License.

**6** **Keras: Multiple Inputs and Mixed Data**. Adrian Rosebrock. (Sample Code) https://www.pyimagesearch.com/2019/02/04/keras-multiple-inputs-and-mixed-data/

**7** **Leaflet**. Vladimir Agafonkin. (Library) Leaflet/Leaflet. https://leafletjs.com/. BSD-2-Clause.

**8** **Maperitive**. Igor Brejc. (Program) https://maperitive.net. Terms apply.

**9** **Meteostat**. Christian Lamprecht. (DB/Library) https://meteostat.net. CC-BY-NC 4.0/MIT-License.

**10** **officialAQIus**. OpenData Stuttgart. Rewritten by Timo Bilhöfer in Python. (Library) https://github.com/opendata-stuttgart/feinstaub-map-v2/blob/master/src/js/feinstaub-api.js. MIT-License.

**11** **OpenStreetMap**. OpenStreetMap contributors. (DB) https://www.openstreetmap.org/copyright. Terms apply.

**12** **Overpass-API**. Wiktorn. (Docker-Image) wiktorn/Overpass-API. AGPL 3.0.

**13** **Pandas**. Pandas contributors. (Library) https://pandas.pydata.org. BSD-3 Clause

**14** **Python**. Python Software Foundation. (Interpreter) https://python.org. PSF-License

**15** **sensor.community**. (DB) https://archive.sensor.community/. Open Data Commons: Database Contents License (DbCL) v1.0.

**16** **Sqlite3**. (Library & DB Language) https://www.sqlite.org. Public Domain.

**17** **TensorFlow**. TensorFlow Community. (Library & Sample Code) https://www.tensorflow.org. Apache-License 2.0.

**18** **VisionAir**. Harshita Diddee, Divyanshu Sharma, Shivam Grover, Shivani Jindal. (DB) https://vision-air.github.io. MIT-License