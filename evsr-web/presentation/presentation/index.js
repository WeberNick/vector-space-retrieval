// Import React
import React from 'react';

// Import Spectacle Core tags
import {
  Appear,
  BlockQuote,
  CodePane,
  Cite,
  Deck,
  Heading,
  ListItem,
  List,
  Quote,
  Slide,
  Text,
  Table,
  TableRow,
  TableHeaderItem,
  TableItem,
  Layout,
  TableHeader,
  TableBody,
  Image,
  MarkdownSlides,
  Markdown,
} from 'spectacle';

// Import theme
import createTheme from 'spectacle/lib/themes/default';

import '../prism-atom-dark.css';
import '../assets/custom.css';

// Require CSS
require('normalize.css');

const theme = createTheme(
  {
    primary: 'white',
    secondary: '#1F2022',
    tertiary: '#03A9FC',
    quarternary: '#CECECE',
  },
  {
    primary: 'Montserrat',
    secondary: 'Helvetica',
  },
);

export default class Presentation extends React.Component {
  render() {
    return (
      <Deck
        transition={['zoom', 'slide']}
        transitionDuration={500}
        theme={theme}
      >
        <Slide transition={['slide']} bgColor="primary">
          <Heading size={1} fit caps lineHeight={1} textColor="secondary">
            Efficient Vector Space Retrieval
          </Heading>
          <Text margin="10px 0 0" textColor="tertiary" size={1} fit bold>
            Making Vector Space Retrieval Lightning Fast ⚡
          </Text>
        </Slide>
        <Slide align="center center" transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            Agenda
          </Heading>
          <List>
            <ListItem>The Problem</ListItem>
            <ListItem>Implementation</ListItem>
            <ListItem>Additional Feature</ListItem>
            <ListItem>Results</ListItem>
          </List>
        </Slide>
        <Slide transition={['slide']} bgColor="tertiary">
          <Heading size={6} textColor="primary" caps>
            The Problem 📝
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            The Problem
          </Heading>
          <List>
            <ListItem>Vanilla Vector Space Model slow</ListItem>
            <ListItem>Faster possibilites ?
              <List>
                <ListItem>
                  Clustering
                </ListItem>
                <ListItem>
                  Tiered Index
                </ListItem>
                <ListItem>
                  Random Projections
                </ListItem>
              </List>
            </ListItem>
            <ListItem>What about retrieval performance ?</ListItem>
            <ListItem>Find tradeoff</ListItem>
          </List>
        </Slide>
        <Slide transition={['slide']} bgColor="tertiary">
          <Heading size={6} textColor="primary" caps>
            Implementation 💻
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <div>
            <Heading
              size={6}
              textColor="tertiary"
              caps
              style={{ marginBottom: '100px' }}
            >
             Core
            </Heading>
            <List>
              <ListItem>Document & DocumentManager</ListItem>
              <ListItem>QueryManager</ListItem>
              <ListItem>IndexManager</ListItem>
              <ListItem>QueryExecutionEngine</ListItem>
          </List>
          </div>
        </Slide>
        <Slide transition={['slide']}>
          <Image src={require('../assets/IR.png')} />
        </Slide>
        <Slide transition={['slide']} bgColor="tertiary">
          <Heading size={6} textColor="primary" caps>
            Additional Feature 💡
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            Additional Feature
          </Heading>
          <List>
            <ListItem>VSM has a fundamental problem</ListItem>
            <ListItem>Documents and queries don't have a high term overlap</ListItem>
            <ListItem>Need some semantic relationship between words</ListItem>
            <ListItem><strong>Word Embeddings</strong> to the rescue...</ListItem>
          </List>
        </Slide>
        <Slide transition={['slide']} bgColor="tertiary">
          <Heading size={6} textColor="primary" caps >
            Results 📊
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            Evaluation
          </Heading>
          <List>
            <ListItem>As expected...
              <List>
                <ListItem>Cluster and Tiered Index fast</ListItem>
                <ListItem>Random Projections add speed up</ListItem>
                <ListItem>VSM best retrieval performance but slow</ListItem>
              </List>
            </ListItem>
          </List>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            Evaluation
          </Heading>
          <Text>Speed up images MAP/DCG/RNT</Text>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            Evaluation
          </Heading>
          <List>
            <ListItem>Did Word Embeddings help ? Problems:
              <List>
                <ListItem>Word Embeddings were built on non preprocessed words</ListItem>
                <ListItem>Word Embeddings were not domain specific</ListItem>
              </List>
            </ListItem>
          </List>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            Evaluation
          </Heading>
          <Text>Graph mit word embeddings</Text>
        </Slide>
        <Slide transition={['slide']} bgColor="tertiary">
          <Heading size={6} textColor="primary" caps >
            Conclusion 📊
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '100px' }}
          >
            Conclusion
          </Heading>
          <Text fit>Best index in terms of runtime & retrieval performance: <br/>
          <strong>Tiered Index with 100 Tiers and Random Projection</strong></Text>
          
        </Slide>
      </Deck>
    );
  }
}
