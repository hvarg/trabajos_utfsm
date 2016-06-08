Los archivos fueron creados con Protégé v5.0.0,
the_simpsons_ontology.owl es el archivo con las relaciones y clases pedidas.
the_simpsons_ontology_OWL2RL.owl es el archivo modificado para poder ser tratado
por el perfil RL.

Para la validación de OWL2RL se utilizó: 
    http://mowl-power.cs.man.ac.uk:8080/validator/
Para hacer que pasara:
* Se borraron las clases que eran equivalentes a una "propiedad some clase".
* Se quitó la transitividad de los hermanos.
* Se quitó la edad.

Nota: El programa al ser una "beta" (al menos en linux) tiene varios problemas,
por ejemplo, cada vez que guardo le agrega a mi IRI "www.example.com/utfsm/"
como prefijo de la misma IRI.
Nota2: No logré que la IRI fuera absoluta.

Hernán Vargas.
