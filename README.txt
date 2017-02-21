--Kurze Spielbeschreibung--
Light Rider, dessen Namensgebung an die beliebte Knight Rider Serie aus den 80er Jahren ihren Ursprung hat, ist ein klassischer Lane Racer, in der der Spieler Hindernissen auf der Fahrbahn durch Wechseln auf anderen Spuren ausweichen soll.

Grafisch soll das Spiel dem 80/90er Jahre Neonstil folgen mit poppigen Farbeffekten und dem richtigen Auto: dem Nicht-Delorean (kann nicht zur�ck in die Zukunft).

11/10 IGN �Sei�e geil�

--Kurze Bedienungsanleitung--
Von der Titelseite aus kommt man mit der Enter Taste zum Spiel.
Spurwechsel wird mit Pfeil Links / A oder Pfeil Rechts / D ausgef�hrt.
W�hrend der Fahrt kann man mit Escape und anschlie�ender Leertaste zur Titelseite zur�ck.

Ziel ist es, m�glichst weit zu kommen, ohne die Hindernisse zu treffen.

--Fremde Quellen--
Gamma korrektur / Licht 
https://learnopengl.com/#!Advanced-Lighting/Gamma-Correction

Render System mit Bloom
https://learnopengl.com/#!Advanced-Lighting/Bloom

AABB Tests
http://www.miguelcasillas.com/?mcportfolio=collision-detection-c

Idee f�r Hintergrund im Men�
https://www.shadertoy.com/view/XlSSWG

--Musikquellen--
Stringed Disco Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 3.0 License
http://creativecommons.org/licenses/by/3.0/

Neon and Kickboxing Kevin MacLeod
Gefunden auf freepd.com
F�llt unter Public Domain.

--Fremde Bibliotheken--
irrKlang 1.5 Sound Bibliothek

--Besonderer Fokus auf bestimmte Funktionen oder elegante L�sungen o. �.--
	HDR Rendering (16 Bit Float Buffer) mit abschlie�endem Tonemapping.
	Licht wird durch Gammakorrektur abgestimmt.
	Zum gro�en Teil Materialien mit emissiven Eigenschaften und Bloom Effekt.
	Ziffern Darstellung, vorgerenderte Textur, Update per uniform.
	Endloses, Zuf�lliges generieren von Strecke. Abschnitte sind durch Bitmaps definiert. 

--Weitere Anmerkungen--
Projekt wurde mit Visual Studio 2015 entwickelt.
Ben�tigt Windows 7+
