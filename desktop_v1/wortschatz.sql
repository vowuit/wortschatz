
-- Table: boxes
CREATE TABLE boxes ( 
    box_id   INTEGER PRIMARY KEY ASC,
    duration TEXT    NOT NULL 
);

INSERT INTO [boxes] ([box_id], [duration]) VALUES (-1, '2 years');
INSERT INTO [boxes] ([box_id], [duration]) VALUES (0, '0 seconds');
INSERT INTO [boxes] ([box_id], [duration]) VALUES (1, '1 minutes');
INSERT INTO [boxes] ([box_id], [duration]) VALUES (2, '12 hour');

-- Table: languages
CREATE TABLE languages ( 
    lang_id INTEGER PRIMARY KEY ASC AUTOINCREMENT,
    name    TEXT    NOT NULL,
    short   TEXT    NOT NULL
                    UNIQUE 
);

INSERT INTO [languages] ([lang_id], [name], [short]) VALUES (1, 'Deutsch', 'de');
INSERT INTO [languages] ([lang_id], [name], [short]) VALUES (2, 'Englisch', 'en');
INSERT INTO [languages] ([lang_id], [name], [short]) VALUES (3, 'Russisch', 'ru');

-- Table: texts
CREATE TABLE texts ( 
    text_id INTEGER PRIMARY KEY ASC,
    lang_id INTEGER NOT NULL,
    text    TEXT    NOT NULL 
);

INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (1, 2, 'Monday');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (2, 1, 'Montag');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (3, 2, 'Tuesday');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (4, 1, 'Dienstag');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (5, 2, 'Wednesday');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (6, 1, 'Mittwoch');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (7, 2, 'Thursday');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (8, 1, 'Donnerstag');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (9, 2, 'Friday');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (10, 1, 'Freitag');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (11, 2, 'Saturday');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (12, 1, 'Sonnabend');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (13, 2, 'Sunday');
INSERT INTO [texts] ([text_id], [lang_id], [text]) VALUES (14, 1, 'Sonntag');

-- Table: translations
CREATE TABLE translations ( 
    trans_id     INTEGER PRIMARY KEY ASC AUTOINCREMENT,
    from_text_id INTEGER NOT NULL,
    to_text_id   INTEGER NOT NULL 
);

INSERT INTO [translations] ([trans_id], [from_text_id], [to_text_id]) VALUES (44, 1, 2);
INSERT INTO [translations] ([trans_id], [from_text_id], [to_text_id]) VALUES (45, 3, 4);
INSERT INTO [translations] ([trans_id], [from_text_id], [to_text_id]) VALUES (46, 5, 6);
INSERT INTO [translations] ([trans_id], [from_text_id], [to_text_id]) VALUES (47, 7, 8);
INSERT INTO [translations] ([trans_id], [from_text_id], [to_text_id]) VALUES (48, 9, 10);
INSERT INTO [translations] ([trans_id], [from_text_id], [to_text_id]) VALUES (49, 11, 12);
INSERT INTO [translations] ([trans_id], [from_text_id], [to_text_id]) VALUES (50, 13, 14);

-- Table: cards
CREATE TABLE cards ( 
    card_id       INTEGER  PRIMARY KEY ASC,
    front_text_id INTEGER  NOT NULL,
    back_text_id  INTEGER  NOT NULL,
    box_id        INTEGER  NOT NULL
                           DEFAULT '0',
    box_in_date   DATETIME NOT NULL
                           DEFAULT ( datetime( 'now' )  ) 
);

INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (1, 1, 2, 0, '2014-12-22 22:24:51');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (2, 2, 1, 1, '2014-12-22 22:30:20');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (3, 3, 4, 1, '2014-12-22 22:30:32');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (4, 4, 3, 1, '2014-12-22 22:30:35');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (5, 5, 6, 0, '2014-12-22 22:31:27');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (6, 6, 5, 0, '2014-12-15 20:13:45');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (7, 7, 8, 0, '2014-12-15 20:13:46');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (12, 12, 11, 0, '2014-12-15 20:13:47');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (13, 13, 14, 0, '2014-12-15 20:13:48');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (14, 14, 13, 0, '2014-12-15 20:13:48');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (15, 8, 7, 0, '2014-12-15 20:13:49');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (16, 9, 10, 0, '2014-12-15 20:13:49');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (17, 10, 9, 0, '2014-12-15 20:13:50');
INSERT INTO [cards] ([card_id], [front_text_id], [back_text_id], [box_id], [box_in_date]) VALUES (18, 11, 12, 0, '2014-12-15 20:13:50');

-- Index: ux_translations
CREATE UNIQUE INDEX ux_translations ON translations ( 
    from_text_id,
    to_text_id 
);


-- Index: ux_cards
CREATE UNIQUE INDEX ux_cards ON cards ( 
    front_text_id,
    back_text_id 
);


-- View: en_de_translations
CREATE VIEW en_de_translations AS
       SELECT translations.trans_id,
              from_texts.text AS from_text,
              to_texts.text AS to_text
         FROM translations
              INNER JOIN texts AS from_texts
                      ON from_texts.text_id = translations.from_text_id
              INNER JOIN languages AS from_lang
                      ON from_lang.lang_id = from_texts.lang_id
              INNER JOIN texts AS to_texts
                      ON to_texts.text_id = translations.to_text_id
              INNER JOIN languages AS to_lang
                      ON to_lang.lang_id = to_texts.lang_id
        WHERE from_lang.short = 'en' 
              AND
              to_lang.short = 'de';
;


-- View: due_text_cards
CREATE VIEW due_text_cards AS
       SELECT cards.card_id AS card_id,
              cards.box_id AS box_id,
              datetime( cards.box_in_date, boxes.duration ) AS box_out_date,
              front.text AS front_text,
              back.text AS back_text
         FROM cards
              INNER JOIN texts AS front
                      ON front.text_id = cards.front_text_id
              INNER JOIN texts AS back
                      ON back.text_id = cards.back_text_id
              INNER JOIN boxes
                      ON boxes.box_id = cards.box_id
        WHERE datetime( 'now', 'localtime' ) >= datetime( cards.box_in_date, boxes.duration );
;

