package dum;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.Serializable;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.*;

public class TermLoad implements Serializable {

    public final String DATA_PATH = "./documents";
    public final String STOP_WORDS_PATH = "./stop_words.txt";

    private HashSet<String> stopWords;
    private LinkedList<String> documents;
    public HashMap<Integer, String> documentos;
    private HashMap<Integer, Integer> maxDocs; // index of doc, max freq in the doc

    private HashMap<String, List<TermValue>> terms;         //inverted list
    private HashMap<String, List<TermValue>> seqTrav;     //sequel travers

    public TermLoad() {
        initializeTermLoad();
    }

    //------------------------------------------------------------------------------------------------------------------

    private void initializeTermLoad() {
        LoadStopWords();
        LoadDocuments();
        Stemmer stemmer = new Stemmer();
        terms = new HashMap<>();
        seqTrav = new HashMap<>();
        maxDocs = new HashMap<>();
        int i = 0;
        // iterate over all documents
        for (String document : documents) {
            final int idx = i++;
            File file = new File(document);
            maxDocs.put(idx, 0);
            // parse words
            Scanner scanner = null;
            try {
                scanner = new Scanner(file).useDelimiter("[^a-zA-Z]+");
            } catch (FileNotFoundException ex) {
                ex.printStackTrace();
            }
            // stemming words and putting them to the temporary hashmap + count terms
            HashMap<String, Integer> tmp = new HashMap<>();
            while (scanner.hasNext()) {
                String word = scanner.next().trim().toLowerCase();
                if (isTerm(word)) {
                    stemmer.add(word.toCharArray(), word.length());
                    stemmer.stem();
                    String term = stemmer.toString();
                    //System.out.println(term);
                    //String term = dum.Stemmer.stem ( word );
                    tmp.put(term, tmp.getOrDefault(term, 0) + 1);
                }
            }
            scanner.close();

            tmp.forEach((String term, Integer count) -> {
                List<TermValue> documentList = terms.getOrDefault(term, new LinkedList<>());
                documentList.add(new TermValue(idx, count, 0));
                terms.put(term, documentList);
                if (count > maxDocs.get(idx))
                    maxDocs.put(idx, count);
            });

        }

        // fill list of dum.TermValue with 0s
        List<TermValue> doc = new ArrayList<>();
        for ( int a = 0; a < documents.size(); a++ )
            doc.add ( new TermValue ( a, 0, 0 ) );

        // compute weight
        terms.forEach ( ( String term, List<TermValue> documentList ) -> {
            double df = documentList.size();
            double x = documents.size() / df;
            double idf = Math.log ( x ) / Math.log ( 2 );

            // put the doc full of 0s to term
            List<TermValue> docs = new ArrayList<>();
            for ( int a = 0; a < documents.size(); a++ )
                docs.add ( new TermValue ( a, 0, 0 ) );
            seqTrav.put ( term, docs );

            for ( TermValue d : documentList ) {
                double tf = d.getFrequency() / maxDocs.get ( d.getIdx() );
                double weight = tf * idf;
                d.setWeight ( weight );

                seqTrav.get ( term ).get ( d.getIdx() ).setWeight ( d.getWeight() );
                seqTrav.get ( term ).get ( d.getIdx() ).setFrequency ( d.getFrequency() );
                seqTrav.get ( term ).get ( d.getIdx() ).setName ( d.getName() );
            }
            Collections.sort ( documentList );  //ascending order
        });

    }

    //------------------------------------------------------------------------------------------------------------------

    private void LoadDocuments() {
        documents = new LinkedList<>();
        documentos = new HashMap<>();
        try {
            Files.walkFileTree ( Paths.get ( DATA_PATH ), new SimpleFileVisitor<Path>() {
                @Override
                public FileVisitResult visitFile ( Path file, BasicFileAttributes attrs ) throws IOException {
                    File f = file.toFile();
                    if ( f.isFile() ) {
                        String path = f.getPath();
                        documents.add ( path );
                        documentos.put(documentos.size(), f.getName());
                    }
                    return FileVisitResult.CONTINUE;
                }
            });
        } catch ( IOException ex ) {
            ex.printStackTrace();
        }
        Collections.sort ( documents );     //ascending order
    }

    //------------------------------------------------------------------------------------------------------------------

    private void LoadStopWords() {
        File stopWordsFile = new File (STOP_WORDS_PATH);
        Scanner scanner = null;
        try {
            scanner = new Scanner ( stopWordsFile ).useDelimiter ( "[^a-zA-Z]+| <.*>" );
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        stopWords = new HashSet<>();
        while ( scanner.hasNextLine() ) {
            stopWords.add ( scanner.nextLine() );
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    public boolean isStopWord ( String word ) {
        return stopWords.contains ( word );
    }

    public boolean isTerm ( String word ) {
        return !isStopWord ( word ) && word.length() > 1;
    }

    public HashMap<String, List<TermValue>> getTerms() {
        return terms;
    }

    public HashMap<String, List<TermValue>> getSeqTrav() {
        return seqTrav;
    }

    public List<String> getDocuments() {
        return documents;
    }


    public HashMap<Integer, String> getDocumentos() {
        return documentos;
    }


    public List<TermValue> getDocValueByTerm ( String term ) {
        return terms.get ( term );
    }

    public static void test(String[] args) throws FileNotFoundException {
        Scanner scanner;
        Stemmer stemmer = new Stemmer();
        File file = new File(args[0]);
        scanner = new Scanner(file).useDelimiter("[^a-zA-Z]+");
        // stemming words and putting them to the temporary hashmap + count terms
        HashMap<String, Integer> tmp = new HashMap<>();
        int wordCount = 0;
        while (scanner.hasNext()) {
            String word = scanner.next().trim().toLowerCase();
            stemmer.add(word.toCharArray(), word.length());
            stemmer.stem();
            String term = stemmer.toString();
            System.out.println(term);
        }
    }
}

