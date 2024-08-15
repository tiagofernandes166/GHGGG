
# Boraginaceae

This is a repository of my work inspired by the news last year of a baby girl that died of organ failure caused by heat, as she was left forgotten inside her parents car.
The link for the news is the following:
https://www.publico.pt/2023/09/12/sociedade/noticia/bebe-ano-encontrado-morto-dentro-carro-campus-faculdade-almada-2063118

"Bebé de 10 meses encontrada morta dentro de carro em campus de faculdade em Almada
O alerta foi dado para o 112 pelo pai, professor na faculdade, que se terá esquecido da filha de 10 meses no carro. A criança frequentava a creche do próprio campus universitário."
https://www.publico.pt/2023/09/12/sociedade/noticia/bebe-ano-encontrado-morto-dentro-carro-campus-faculdade-almada-2063118


The name given to the project is the same of the "forget me not" flower, original from Western Eurasia.

I've tested lots of diferent choices and sensors, but after some work decided to use the Seed Studio 24GHz mmWave Sensor for XIAO - Human Static Presence, but instead of the XIAO platform, I've ported the library's to ESP32-C3 so there are more digital IO Pins availiable to interface with the car.

Also as it is to be made with off the shelf parts, the interface to the car is the Hazzard Lights, with a voltage step-down from 12vdc to 3.3vdc, and an interface circuit so it reads the hazard lights activity to enable a countdown timer of 120seconds, before start mesuring.

The calibration must be done with the car without any mamal inside, including humans, and thus it allows calibration to be done once via bluetooth and using a speacial toolkit.

it uses ATSHA204A ic's in some parts, as some features need some extra security layer and thus it is provided.

It interfaces with the car windows, and has pins availiable to be interfaced to any OBDII standard system, but to simplify it works interfaced only with the hazard lights.

It also uses a mobile phone circuit with SIM Card, and a meshtastic device to be able to comunicate longer distances without network coverage.

There are 8 GPIO pins availiable for further interfacing, and it also suppots its own 3.7V 250mAh Li-Po battery and recharging circuit.

Some inspiration cames from the youtuber and Maker AndreasSpiess, on whis video series about human presence sensors.


## Etiquetas

Adicione etiquetas de algum lugar, como: [shields.io](https://shields.io/)

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)


