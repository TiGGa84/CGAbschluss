--Kurze Spielbeschreibung--
Light Rider, dessen Namensgebung an die beliebte Knight Rider Serie aus den 80er Jahren ihren Ursprung hat, ist ein klassischer Lane Racer, in der der Spieler Hindernissen auf der Fahrbahn durch Wechseln auf anderen Spuren ausweichen soll.

Grafisch soll das Spiel dem 80/90er Jahre Neonstil folgen mit poppigen Farbeffekten und dem richtigen Auto: dem Nicht-Delorean (kann nicht zurück in die Zukunft).

11/10 IGN “Seiße geil”

--Kurze Bedienungsanleitung--
Von der Titelseite aus kommt man mit der Enter Taste zum Spiel.
Spurwechsel wird mit Pfeil Links / A oder Pfeil Rechts / D ausgeführt.
Während der Fahrt kann man mit Escape und anschließender Leertaste zur Titelseite zurück.

Ziel ist es, möglichst weit zu kommen, ohne die Hindernisse zu treffen.

--Fremde Quellen--
Gamma korrektur / Licht 
https://learnopengl.com/#!Advanced-Lighting/Gamma-Correction

Render System mit Bloom
https://learnopengl.com/#!Advanced-Lighting/Bloom

AABB Tests
http://www.miguelcasillas.com/?mcportfolio=collision-detection-c

Idee für Hintergrund im Menü
https://www.shadertoy.com/view/XlSSWG

--Musikquellen--
Stringed Disco Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 3.0 License
http://creativecommons.org/licenses/by/3.0/

Neon and Kickboxing Kevin MacLeod
Gefunden auf freepd.com
Fällt unter Public Domain.

--Fremde Bibliotheken--
irrKlang 1.5 Sound Bibliothek

--Besonderer Fokus auf bestimmte Funktionen oder elegante Lösungen o. Ä.--
	HDR Rendering (16 Bit Float Buffer) mit abschließendem Tonemapping.
	Licht wird durch Gammakorrektur abgestimmt.
	Zum großen Teil Materialien mit emissiven Eigenschaften und Bloom Effekt.
	Ziffern Darstellung, vorgerenderte Textur, Update per uniform.
	Endloses, Zufälliges generieren von Strecke. Abschnitte sind durch Bitmaps definiert. 

--Weitere Anmerkungen--
Projekt wurde mit Visual Studio 2015 entwickelt.
Benötigt Windows 7+
