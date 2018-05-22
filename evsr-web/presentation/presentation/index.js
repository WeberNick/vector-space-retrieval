// Import React
import React from 'react';
import slidesMarkdown from 'raw-loader!../assets/markdown.md';

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

const images = {
  city: require('../assets/city.jpg'),
  kat: require('../assets/kat.png'),
  logo: require('../assets/formidable-logo.svg'),
  markdown: require('../assets/markdown.png'),
};

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
        <Slide align="center center" transition={['slide']} bgColor="tertiary">
          <Heading size={6} textColor="primary" caps>
            Agenda
          </Heading>
          <List textColor="primary">
            <ListItem>Task</ListItem>
            <ListItem>Implementation</ListItem>
            <ListItem>Additional Feature</ListItem>
            <ListItem>Results</ListItem>
          </List>
        </Slide>
        <Slide transition={['slide']}>
          <Heading size={6} textColor="secondary" caps>
            The Task 📝
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading size={6} textColor="secondary" caps>
            Implementation 💻
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading size={6} textColor="secondary" caps>
            Additional Feature 💡
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading size={6} textColor="secondary" caps>
            Result 📊
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
              Building following indexes
            </Heading>
            <List textColor="secondary">
              <ListItem>Inverted Index</ListItem>
              <ListItem>Tiered Index</ListItem>
              <ListItem>Clustered Index</ListItem>
              <ListItem>Results</ListItem>
            </List>
          </div>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '30px' }}
          >
            Building the indexes
          </Heading>
          <CodePane
            lang="cpp"
            source={require('raw-loader!../assets/index_manager_init.example')}
            theme="external"
            margin="20px auto"
            overflow="overflow"
          />
          <Text margin="10px 0 0" textColor="secondary" lineHeight={0.5}>
            Builds every index we need during retrieval
          </Text>
        </Slide>
        <Slide transition={['slide']}>
          <Heading size={6} textColor="secondary" caps>
            Additional Feature: Word embeddings 📝
          </Heading>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '30px' }}
          >
            Evaluation
          </Heading>
        </Slide>
      </Deck>
    );
  }
}
